#include <fin/readers/dummy_reader.h>

const std::vector<float> &fin::readers::DummyReader::getData() {
    return this->data;
}

void fin::readers::DummyReader::addSamples(float *samples, std::size_t size) {
    for (std::size_t i = 0; i < size; i++) {
        this->data.push_back(samples[i]);
    }
}

std::size_t fin::readers::DummyReader::getLen() {
    return this->data.size();
}

void fin::readers::DummyReader::dropSamples() {
    this->data.resize(0);
}