#ifndef WASM_DUMMY_READER_H
#define WASM_DUMMY_READER_H

#include <cstdint>
#include <fin/readers/reader.h>

class dummy_reader : public fin::readers::Reader {
private:
    std::vector<float> data{};

public:
    void add_samples(float* samples, std::size_t size);

    const std::vector<float> &getData() override;

    void dropSamples() override;

    std::size_t get_len();
};

#endif
