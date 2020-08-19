#ifndef IO_WAVREADER_H
#define IO_WAVREADER_H

#include <string>
#include <vector>
#include <cstdint>

struct Chunk;

namespace IO {
    /**
     * Class to read WAV files
     */
    class WavReader {

    private:
        std::string fileName;
    public:
        [[nodiscard]] const std::string &getFileName() const;

    private:
        std::vector<float> data;
    public:
        explicit WavReader(const std::string &fileName);

        const std::vector<float> &getData();

    private:
        static bool
        findChunk(const uint8_t *id, Chunk &chunk, std::ifstream &wavFile, const bool &isBigEndian, bool iterate);
    };
}

#endif
