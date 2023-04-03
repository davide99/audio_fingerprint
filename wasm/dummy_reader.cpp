#include "dummy_reader.h"

const std::vector<float> &dummy_reader::get_data() {
    return this->data;
}

void dummy_reader::add_samples(float *samples, std::size_t size) {
    for (std::size_t i = 0; i < size; i++) {
        this->data.push_back(samples[i]);
    }
}

std::size_t dummy_reader::get_len() {
    return this->data.size();
}

void dummy_reader::drop_samples() {
    this->data.resize(0);
}