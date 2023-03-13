#include "IO/Readers/DummyReader.h"
#include "Utils/Wrapper.h"

extern "C" {

float compute(const float *samples, const int len) {
    auto reader = IO::Readers::DummyReader(samples, len);
    auto links = Utils::computeLinks(reader);

    //A questo punto dobbiamo ritornare i link, ma non possiamo ritornare dati "complessi"


    return static_cast<float>(links[0].getHash());
}

int main(){
    float array[] = {0, 0, 0};

    compute(array, 3);
}

}