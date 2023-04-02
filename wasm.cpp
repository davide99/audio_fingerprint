#include <emscripten/webaudio.h>
#include <string>
#include <cstdint>
#include "IO/Readers/DummyReader.h"
#include "Utils/Wrapper.h"
#include "Consts.h"

//shared variable between main thread and audio thread
std::int8_t start = 0;

//variabili del solo audio thread
constexpr auto SAMPLE_SEC = 4; //numero secondi durata sample
constexpr auto NUM_SAMPLES = Consts::Audio::SampleRate * SAMPLE_SEC;
constexpr auto PROCESS_SAMPLES = 128;

IO::Readers::DummyReader dummyReader;
std::int8_t first_time = 1;

void messageReceivedOnMainThread() {
    auto links = Utils::computeLinks(dummyReader);
    dummyReader.dropSamples();

    std::string json = "[";
    for (auto &link: links) {
        json += "{\"hash\":" + std::to_string(link.getHash()) +
                ", \"window\":" + std::to_string(link.getTime()) + "},";
    }
    json.pop_back();
    json += "]";

    EM_ASM({
               console.log(UTF8ToString($0));
           }, json.c_str());
}


EM_BOOL processAudio(
        int numInputs, const AudioSampleFrame *inputs,
        int numOutputs, AudioSampleFrame *outputs,
        int numParams, const AudioParamFrame *params,
        void *userData
) {
    //Unused
    (void) numInputs, (void) numOutputs, (void) outputs, (void) numParams, (void) params, (void) userData;

    //Se non l'ho avviato allora esco subito
    if (!start) return EM_TRUE;

    if (dummyReader.getLen() < NUM_SAMPLES) {
        //Assumiamo numero input == 1
        dummyReader.addSamples(inputs->data, PROCESS_SAMPLES);
    } else if (first_time) {
        first_time = 0;

        emscripten_audio_worklet_post_function_v(EMSCRIPTEN_AUDIO_MAIN_THREAD, messageReceivedOnMainThread);

        return EM_FALSE;
    }

    // We want to keep this processor going. Return EM_FALSE here to shut down.
    return EM_TRUE;
}

// This callback will fire after the Audio Worklet Processor has finished being added to the Worklet global scope.
void audioWorkletProcessorCreated(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, void *userData) {
    (void) userData;

    if (!success) return;

    EmscriptenAudioWorkletNodeCreateOptions options = {
            .numberOfInputs = 1,
            .numberOfOutputs = 0,
    };

    // Instantiate the node Audio Worklet Processor.
    EMSCRIPTEN_AUDIO_WORKLET_NODE_T wasmAudioWorklet = emscripten_create_wasm_audio_worklet_node(
            audioContext, "node", &options, &processAudio, nullptr
    );

    EM_ASM({
               const audioContext = emscriptenGetAudioObject($0);
               audioContext.suspend();

               // Add a button on the page to toggle playback as a response to user click.
               const startButton = document.createElement('button');
               startButton.innerHTML = 'Toggle playback';
               document.body.appendChild(startButton);

               const audioWorkletNode = emscriptenGetAudioObject($1);
               const oscillator = new OscillatorNode(audioContext, {type: 'sine', frequency: 440});
               oscillator.start();

               oscillator.connect(audioWorkletNode);
               oscillator.connect(audioContext.destination);

               startButton.onclick = () => {
            if (audioContext.state != 'running') {
                audioContext.resume();
                HEAP8[$2] = 1;
            } else {
                audioContext.suspend();
                HEAP8[$2] = 0;
            }
        };
           }, audioContext, wasmAudioWorklet, &start);
}

/*
 * This callback will fire when the Wasm Module has been shared to the AudioWorklet global scope,
 * and is now ready to begin adding Audio Worklet Processors.
*/
void webAudioWorkletThreadInitialized(EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, void *userData) {
    (void) userData;

    if (!success) return;

    WebAudioWorkletProcessorCreateOptions opts = {
            .name = "node",
    };
    emscripten_create_wasm_audio_worklet_processor_async(audioContext, &opts, audioWorkletProcessorCreated, nullptr);
}

/*
 * Define a global stack space for the AudioWorkletGlobalScope.
 * Note that all AudioWorkletProcessors and/or AudioWorkletNodes on the given Audio Context all share the same
 * AudioWorkerGlobalScope, i.e. they all run on the same one audio thread (multiple nodes/processors do not each
 * get their own thread). Hence one stack is enough.
 */
uint8_t wasmAudioWorkletStack[134217728];

int main() {
    // Create an audio context
    EmscriptenWebAudioCreateAttributes attrs = {
            .latencyHint = "interactive",
            .sampleRate = Consts::Audio::SampleRate
    };

    EMSCRIPTEN_WEBAUDIO_T context = emscripten_create_audio_context(&attrs);

    /*
     * and kick off Audio Worklet scope initialization, which shares the Wasm Module and Memory to the AudioWorklet
     * scope and initializes its stack.
     */
    emscripten_start_wasm_audio_worklet_thread_async(
            context,
            wasmAudioWorkletStack, sizeof(wasmAudioWorkletStack),
            webAudioWorkletThreadInitialized, nullptr);
}