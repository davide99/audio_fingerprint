#ifndef DUMMYREADER_H
#define DUMMYREADER_H

#include <cstdint>
#include "Reader.h"

namespace IO::Readers {
    class DummyReader : public Reader {
    private:
        std::vector<float> data;

    public:
        DummyReader(const float *samples, std::size_t size);

        const std::vector<float> &getData() override;
    };
}

#endif
