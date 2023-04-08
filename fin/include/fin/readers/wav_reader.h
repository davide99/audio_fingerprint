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
        std::string filename;

    private:
        std::vector<float> data;
    public:
        explicit WavReader(const std::string &filename);

        const std::vector<float> &getData() override;

        void dropSamples() override;

    private:
        static bool
        findChunk(const uint8_t *id, Chunk &chunk, std::ifstream &wav_file, const bool &is_big_endian, bool iterate);
    };
}
#endif
