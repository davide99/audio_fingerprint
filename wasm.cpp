#include "emscripten.h"
#include <cstdint>
#include <iostream>
#include "emscripten.h"
#include <cstdlib>
//#include "Utils/Wrapper.h"
//#include "IO/Readers/DummyReader.h"

extern "C" {

EMSCRIPTEN_KEEPALIVE
float compute(const float *samples, int len) {
    //IO::Readers::DummyReader reader(samples, len);
    //auto links = Utils::computeLinks(reader);

    //A questo punto dobbiamo ritornare i link, ma non possiamo ritornare dati "complessi"

    float sum = 0.0f;
    
    for (int i = 0; i < len; i++)
        sum += samples[i];

    return sum;
}

}