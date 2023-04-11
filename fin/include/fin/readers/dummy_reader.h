#ifndef FIN_READERS_DUMMY_READER_H
#define FIN_READERS_DUMMY_READER_H

#include <cstdint>
#include <fin/readers/reader.h>

namespace fin::readers {
    class DummyReader : public fin::readers::Reader {
    private:
        std::vector<float> data_{};

    public:
        void addSamples(float *samples, std::size_t size);

        const std::vector<float> &getData() override;

        void dropSamples() override;

        std::size_t getLen();
    };
}

#endif
