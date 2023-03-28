#include "DummyReader.h"

namespace IO::Readers {
    const std::vector<float> &DummyReader::getData() {
        return this->data;
    }

    void DummyReader::addSamples(float *samples, std::size_t size) {
        for (std::size_t i = 0; i < size; i++) {
            this->data.push_back(samples[i]);
        }
    }

    std::size_t DummyReader::getLen() {
        return this->data.size();
    }
}