#include <fin/readers/wav_reader.h>
#include <fin/math/integers.h>
#include <fin/utils/utils.h>
#include "../../consts.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

/*
 * http://soundfile.sapp.org/doc/WaveFormat/
 * https://sites.google.com/site/musicgapi/technical-documents/wav-file-format#wavefileheader
 */

constexpr auto id_size = 4u;

#pragma pack(push, 1)
struct chunk { //Generic chunk
    std::uint8_t id[id_size];
    std::uint32_t size;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct fmt_chunk {
    //chunk defined above
    std::uint16_t audio_format;
    std::uint16_t channels;
    std::uint32_t sample_rate;
    [[maybe_unused]] std::uint32_t average_byte_per_sec;
    [[maybe_unused]] std::uint16_t block_align;
    std::uint16_t bits_per_sample;
};
#pragma pack(pop)


//chunk IDs
constexpr std::uint8_t riff_id[] = {0x52u, 0x49u, 0x46u, 0x46u}; //RIFF
constexpr std::uint8_t wave_id[] = {0x57u, 0x41u, 0x56u, 0x45u}; //WAVE
constexpr std::uint8_t fmt_id[] = {0x66u, 0x6Du, 0x74u, 0x20u};  //fmt_
constexpr std::uint8_t data_id[] = {0x64u, 0x61u, 0x74u, 0x61u}; //DATA

//Check if the WAV has the correct characteristics
static bool check_fmt_chunk(const fmt_chunk &fmtChunk) {
    return (fmtChunk.audio_format == 1u) &&                  //PCM
           (fmtChunk.channels == consts::audio::CHANNELS) &&
           (fmtChunk.sample_rate == consts::audio::SAMPLE_RATE) &&
           (fmtChunk.bits_per_sample == consts::audio::BITS_PER_SAMPLE);
}

//Iterate through the WAV file to find a chunk given the id
bool
fin::readers::wav_reader::find_chunk(const std::uint8_t *id, chunk &chunk, std::ifstream &wav_file, const bool &is_big_endian,
                       bool iterate = true) {
    bool found;

    do {
        try {
            wav_file.read(reinterpret_cast<char *>(&chunk), sizeof(chunk)); //Read a chunk
        } catch (const std::ifstream::failure &e) {
            return false;
        }

        if (is_big_endian)
            chunk.size = fin::math::integers::byte_swap(chunk.size);

        found = !std::memcmp(chunk.id, id, id_size); //Compare with the given id

        if (!found)
            try {
                //seek from the current position ahead of chunk.size bytes
                wav_file.seekg(chunk.size, std::ios_base::cur);
            } catch (const std::ifstream::failure &e) {
                return false;
            }

    } while (iterate && !found);

    return true;
}

fin::readers::wav_reader::wav_reader(const std::string &filename) {
    chunk chunk{};
    bool is_big_endian = fin::utils::is_big_endian();

    std::ifstream wav_file(filename, std::ios::binary);
    wav_file.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
    if (wav_file.fail()) {
        throw std::runtime_error("Can't open file: " + filename);
    }

    //Read riff chunk
    {
        if (!this->find_chunk(riff_id, chunk, wav_file, is_big_endian, false))
            throw std::runtime_error(filename + " is not a WAV file");

        std::uint8_t riff_type[id_size];
        wav_file.read(reinterpret_cast<char *>(&riff_type), sizeof(riff_type));

        if (std::memcmp(riff_type, wave_id, id_size) != 0)
            throw std::runtime_error("Invalid RIFF chunk in: " + filename);
    }

    //Read fmt chunk, it might not be immediately after the riff chunk
    {
        if (!this->find_chunk(fmt_id, chunk, wav_file, is_big_endian))
            throw std::runtime_error(filename + " is malformed");

        fmt_chunk fmt_chunk{};
        wav_file.read(reinterpret_cast<char *>(&fmt_chunk), sizeof(fmt_chunk));

        if (!check_fmt_chunk(fmt_chunk))
            throw std::runtime_error("WAV is not in the correct format");
    }

    std::int64_t number_of_samples;

    //Read data chunk
    {
        if (!this->find_chunk(data_id, chunk, wav_file, is_big_endian))
            throw std::runtime_error(filename + " is malformed");

        number_of_samples = (chunk.size << 3u) / (consts::audio::CHANNELS * consts::audio::BITS_PER_SAMPLE);
    }

    //For whatever reason using a vector instead of a plain old array makes valgrind not complain
    std::vector<std::int16_t> i_data;
    i_data.resize(number_of_samples); //Resize to avoid relocations
    this->data.resize(number_of_samples);

    //Read the samples
    wav_file.read(reinterpret_cast<char *>(i_data.data()), number_of_samples * consts::audio::BITS_PER_SAMPLE >> 3u);

    if (is_big_endian)
        for (auto &sample: i_data)
            sample = fin::math::integers::byte_swap(sample);

    //Convert the int16 to a float
    std::transform(i_data.begin(), i_data.end(), this->data.data(), [](const std::int16_t &i) -> float {
        return i;
    });

    wav_file.close();
}

const std::vector<float> &fin::readers::wav_reader::get_data() {
    return this->data;
}

void fin::readers::wav_reader::drop_samples() {
    this->data.resize(0);
}
