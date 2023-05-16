#include "../consts.h"
#include <emscripten/webaudio.h>
#include <emscripten/fetch.h>
#include <fin/readers/dummy_reader.h>
#include <fin/fin.h>
#include <memory>
#include <iostream>

//shared variable between main thread and audio thread
std::int8_t start = 0;

//variabili del solo audio thread
constexpr auto SAMPLE_SEC = 8; //numero secondi durata sample
constexpr auto NUM_SAMPLES = consts::audio::SAMPLE_RATE * SAMPLE_SEC;
constexpr auto PROCESS_SAMPLES = 128;

fin::readers::DummyReader dummyReader;
std::int8_t first_time = 1;
std::unique_ptr<std::uint8_t[]> data;

void messageReceivedOnMainThread() {
    auto links = fin::computeLinks(dummyReader);
    dummyReader.dropSamples();

    auto byteBuffer = links.toByteBuffer();
    data = std::move(byteBuffer.getPtr());

    //Post verso server con links
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);

    static const char *const headers[] = {
            "Content-Type",
            consts::rest::CONTENT_TYPE_BINARY.c_str(),
            nullptr //fine
    };
    attr.requestHeaders = headers;
    std::cout << "Bytes: " << byteBuffer.getSize() << std::endl;
    std::cout << "Links: " << links.size() << std::endl;
    attr.requestData = reinterpret_cast<const char *>(data.get());
    attr.requestDataSize = byteBuffer.getSize();
    std::strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = [](emscripten_fetch_t *fetch) {
        std::string ret(fetch->data, fetch->totalBytes);

        EM_ASM({
            const response_string = UTF8ToString($0);
            if (response_string) {
                const response_json = JSON.parse(response_string);

                if (Object.hasOwn(response_json, 'song_name')) {
                    console.log(response_json.song_name);
                }
            }
        }, ret.c_str());

        data.reset();
        emscripten_fetch_close(fetch); // Free data associated with the fetch.
    };
    attr.onerror = [](emscripten_fetch_t *fetch) {
        std::cout << "POST at " << fetch->url << " failed" << std::endl;
        data.reset();
        emscripten_fetch_close(fetch); // Also free data on failure.
    };
    emscripten_fetch(&attr, consts::rest::FULL_SEARCH_ENDPOINT.c_str());
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
               function init(stream){
                   const audioContext = emscriptenGetAudioObject($0);
                   const mic = audioContext.createMediaStreamSource(stream);

                   // Add a button on the page to toggle playback as a response to user click.
                   const startButton = document.createElement('button');
                   startButton.innerHTML = 'Record';
                   document.body.appendChild(startButton);

                   startButton.onclick = () => {
                       if (audioContext.state != 'running') {
                           audioContext.resume();
                           const audioWorkletNode = emscriptenGetAudioObject($1);
                           mic.connect(audioWorkletNode);

                           HEAP8[$2] = 1;
                       } else {
                           audioContext.suspend();
                           HEAP8[$2] = 0;
                       }
                   };
               }

        navigator.mediaDevices.getUserMedia({audio: {echoCancellation: false, noiseSuppression: false, channelCount: 1}}).then((stream) => init(stream));

        }, audioContext, wasmAudioWorklet, &start);
}

/*
 * Define a global stack space for the AudioWorkletGlobalScope.
 * Note that all AudioWorkletProcessors and/or AudioWorkletNodes on the given Audio Context all share the same
 * AudioWorkerGlobalScope, i.e. they all run on the same one audio thread (multiple nodes/processors do not each
 * get their own thread). Hence one stack is enough.
 */
std::uint8_t wasmAudioWorkletStack[4096];

int main() {
    // Create an audio context
    EmscriptenWebAudioCreateAttributes attrs = {
            .latencyHint = "interactive",
            .sampleRate = consts::audio::SAMPLE_RATE
    };

    EMSCRIPTEN_WEBAUDIO_T context = emscripten_create_audio_context(&attrs);

    /*
     * and kick off Audio Worklet scope initialization, which shares the Wasm Module and Memory to the AudioWorklet
     * scope and initializes its stack.
     */
    emscripten_start_wasm_audio_worklet_thread_async(
            context,
            wasmAudioWorkletStack, sizeof(wasmAudioWorkletStack),
            [](EMSCRIPTEN_WEBAUDIO_T audioContext, EM_BOOL success, void *userData) {
                (void) userData;

                // This callback will fire when the Wasm Module has been shared to the AudioWorklet global scope,
                // and is now ready to begin adding Audio Worklet Processors.

                if (!success) return;

                WebAudioWorkletProcessorCreateOptions opts = {.name = "node"};
                emscripten_create_wasm_audio_worklet_processor_async(
                        audioContext, &opts, audioWorkletProcessorCreated, nullptr);
            },
            nullptr);
}