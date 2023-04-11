#include <fin/readers/dummy_reader.h>

const std::vector<float> &fin::readers::DummyReader::getData() {
    return data_;
}

void fin::readers::DummyReader::addSamples(float *samples, std::size_t size) {
    for (std::size_t i = 0; i < size; i++) {
        data_.push_back(samples[i]);
    }
}

std::size_t fin::readers::DummyReader::getLen() {
    return data_.size();
}

void fin::readers::DummyReader::dropSamples() {
    data_.resize(0);
}