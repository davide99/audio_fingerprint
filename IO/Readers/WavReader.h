#ifndef IO_WAVREADER_H
#define IO_WAVREADER_H

#include <string>
#include <vector>
#include <cstdint>
#include "Reader.h"

struct Chunk;

namespace IO::Readers {
    /**
     * Class to read WAV files
     */
    class WavReader : public Reader {

    private:
        std::string fileName;

    private:
        std::vector<float> data;
    public:
        explicit WavReader(const std::string &fileName);

        const std::vector<float> &getData() override;

    private:
        static bool
        findChunk(const uint8_t *id, Chunk &chunk, std::ifstream &wavFile, const bool &isBigEndian, bool iterate);
    };
}

#endif
