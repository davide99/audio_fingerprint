#ifndef DUMMYREADER_H
#define DUMMYREADER_H

#include <cstdint>
#include <fin/readers/reader.h>

class dummy_reader : public fin::readers::reader {
private:
    std::vector<float> data{};

public:
    void add_samples(float* samples, std::size_t size);

    const std::vector<float> &get_data() override;

    void drop_samples() override;

    std::size_t get_len();
};

#endif
