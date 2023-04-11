#ifndef FIN_READERS_WAVREADER_H
#define FIN_READERS_WAVREADER_H

#include <string>
#include <vector>
#include <cstdint>
#include <fin/readers/reader.h>

struct Chunk;

/**
 * Class to read WAV files
 */
namespace fin::readers {
    class WavReader : public fin::readers::Reader {

    private:
        std::string basename_;
        std::vector<float> data;

    public:
        explicit WavReader(const std::string &filename);

        const std::vector<float> &getData() override;

        void dropSamples() override;

        const std::string &getBasename();

    private:
        static bool
        findChunk(const uint8_t *id, Chunk &chunk, std::ifstream &wavFile, const bool &isBigEndian, bool iterate);
    };
}
#endif
