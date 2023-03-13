#include "DummyReader.h"

namespace IO::Readers {
    DummyReader::DummyReader(const float *samples, const size_t size) {
        this->data.insert(data.end(), samples, samples + size);
    }

    const std::vector<float> &DummyReader::getData() {
        return this->data;
    }
}