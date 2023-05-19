#include "../../consts.h"
#include <emscripten/webaudio.h>
#include <emscripten/bind.h>
#include <fin/readers/dummy_reader.h>
#include <fin/fin.h>
#include <memory>
#include <iostream>
#include <chrono>

//shared variable between main thread and audio thread
std::int8_t start = 0;

//variabili del solo audio thread
constexpr auto SAMPLE_SEC = 8; //numero secondi durata sample
constexpr auto NUM_SAMPLES = consts::audio::SAMPLE_RATE * SAMPLE_SEC;
constexpr auto PROCESS_SAMPLES = 128;

fin::readers::DummyReader dummyReader;
std::int8_t first_time = 1;
std::unique_ptr<std::uint8_t[]> data;
std::chrono::system_clock::time_point firstSampleTime = std::chrono::system_clock::time_point::min();

float getElapsedTime() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<float> diff = now - firstSampleTime;
    return diff.count();
}

EMSCRIPTEN_BINDINGS(my_module){
    emscripten::function("getElapsedTime", &getElapsedTime);
}

void messageReceivedOnMainThread() {
    auto links = fin::computeLinks(dummyReader);
    dummyReader.dropSamples();

    auto byteBuffer = links.toByteBuffer();
    data = std::move(byteBuffer.getPtr());
    std::cout << "Bytes: " << byteBuffer.getSize() << std::endl;
    std::cout << "Links: " << links.size() << std::endl;

    //Post verso server con links
    EM_ASM({
        const searchEndpointUrl = UTF8ToString($0);
        const data = new Uint8Array(HEAPU8.buffer, $1, $2);

        fetch(searchEndpointUrl, {
            method: 'POST',
            body: data,
            headers: {
                'Content-Type': UTF8ToString($3)
            }
        })
        .then(response => {
            if (response.status === 200) {
                return response.json();
            } else {
                throw new Error('Error searching song');
            }
        })
        .then(jsonData => {
            //Qui abbiamo le informazioni sulla canzone: dobbiamo scaricare il testo
            const songId = 1;
            const lyricsUrl = `/lyrics/${songId}`;

            fetch(lyricsUrl)
            .then(response => {
                if (response.status === 200) {
                    return response.json();
                } else {
                    throw new Error('Error fetching lyrics');
                }
            })
            .then(lyricsJson => lyricsJson.lyrics)
            .then(lyrics => {
                const lyricsText = document.getElementById("lyrics-text");
                const lyricsTextNext = document.getElementById("lyrics-text-next");
                const elapsedTime = Module.getElapsedTime();

                for (let i = 0; i < lyrics.length; i++){
                    const currentLine = lyrics[i];
                    const nextLine = i < lyrics.length - 1 ? lyrics[i+1] : "";
                    const millisAt = (currentLine.offset - elapsedTime - jsonData.time_delta) * 1000;
                    if (millisAt >= 0) {
                        setTimeout(() => {
                            lyricsText.innerHTML = currentLine.text;
                            if (nextLine) {
                                lyricsTextNext.style.display = "block";
                                lyricsTextNext.innerHTML = nextLine.text;
                            } else {
                                lyricsTextNext.style.display = "none";
                            }
                        }, millisAt);
                    }
                }
            })
            .catch(error => console.error(error));
        })
        .catch(error => console.error(error));
    }, consts::rest::FULL_SEARCH_ENDPOINT.c_str(), data.get(), byteBuffer.getSize(), consts::rest::CONTENT_TYPE_BINARY.c_str());


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
        if (firstSampleTime == std::chrono::system_clock::time_point::min())
            firstSampleTime = std::chrono::system_clock::now();

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
       const lyricsText = document.getElementById("lyrics-text");
       const lyricsTextNext = document.getElementById("lyrics-text-next");
       lyricsTextNext.style.display = "none";

       function init(stream){
           const audioContext = emscriptenGetAudioObject($0);
           const mic = audioContext.createMediaStreamSource(stream);

           const clickListener = (event) => {
               event.currentTarget.removeEventListener('click', clickListener);
               lyricsText.style.cursor = "auto";
               lyricsText.innerHTML = "Sto riconoscendo la canzone...";

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
           lyricsText.addEventListener('click', clickListener);
       }

       navigator.mediaDevices
            .getUserMedia({audio: {echoCancellation: false, noiseSuppression: false, channelCount: 1}})
            .then((stream) => init(stream));
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