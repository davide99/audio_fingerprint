#ifndef DUMMYREADER_H
#define DUMMYREADER_H

#include <cstdint>
#include "Reader.h"

namespace IO::Readers {
    class DummyReader : public Reader {
    private:
        std::vector<float> data{};

    public:
        void addSamples(float* samples, std::size_t size);

        const std::vector<float> &getData() override;

        void dropSamples() override;

        std::size_t getLen();
    };
}

#endif
