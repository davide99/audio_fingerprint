#ifndef IO_WAVREADER_H
#define IO_WAVREADER_H

#include <string>
#include <vector>
#include <cstdint>
#include <fin/readers/reader.h>

struct chunk;

/**
 * Class to read WAV files
 */
class wav_reader : public fin::readers::reader {

    private:
        std::string filename;

    private:
        std::vector<float> data;
    public:
        explicit wav_reader(const std::string &filename);

        const std::vector<float> &get_data() override;

        void drop_samples() override;

    private:
        static bool
        find_chunk(const uint8_t *id, chunk &chunk, std::ifstream &wav_file, const bool &is_big_endian, bool iterate);
    };

#endif
