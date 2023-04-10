#include "../../consts.h"
#include <fin/readers/wav_reader.h>
#include <fin/math/integers.h>
#include <fin/utils/utils.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

/*
 * http://soundfile.sapp.org/doc/WaveFormat/
 * https://sites.google.com/site/musicgapi/technical-documents/wav-file-format#wavefileheader
 */

constexpr auto idSize = 4u;

#pragma pack(push, 1)
struct Chunk { //Generic chunk
    std::uint8_t id[idSize];
    std::uint32_t size;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FmtChunk {
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
constexpr std::uint8_t riffId[] = {0x52u, 0x49u, 0x46u, 0x46u}; //RIFF
constexpr std::uint8_t waveId[] = {0x57u, 0x41u, 0x56u, 0x45u}; //WAVE
constexpr std::uint8_t fmtId[] = {0x66u, 0x6Du, 0x74u, 0x20u};  //fmt_
constexpr std::uint8_t dataId[] = {0x64u, 0x61u, 0x74u, 0x61u}; //DATA

//Check if the WAV has the correct characteristics
static bool checkFmtChunk(const FmtChunk &fmtChunk) {
    return (fmtChunk.audio_format == 1u) &&                  //PCM
           (fmtChunk.channels == consts::audio::CHANNELS) &&
           (fmtChunk.sample_rate == consts::audio::SAMPLE_RATE) &&
           (fmtChunk.bits_per_sample == consts::audio::BITS_PER_SAMPLE);
}

//Iterate through the WAV file to find a chunk given the id
bool
fin::readers::WavReader::findChunk(const std::uint8_t *id, Chunk &chunk, std::ifstream &wav_file,
                                   const bool &is_big_endian,
                                   bool iterate = true) {
    bool found;

    do {
        try {
            wav_file.read(reinterpret_cast<char *>(&chunk), sizeof(chunk)); //Read a chunk
        } catch (const std::ifstream::failure &e) {
            return false;
        }

        if (is_big_endian)
            chunk.size = fin::math::integers::byteSwap(chunk.size);

        found = !std::memcmp(chunk.id, id, idSize); //Compare with the given id

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

fin::readers::WavReader::WavReader(const std::string &filename) {
    Chunk chunk{};
    bool isBigEndian = fin::utils::isBigEndian();
    this->basename = fin::utils::getBasename(filename);

    if (this->basename.empty()) {
        throw std::runtime_error("Can't extract basename from: " + filename);
    }

    std::ifstream wavFile(filename, std::ios::binary);
    wavFile.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
    if (wavFile.fail()) {
        throw std::runtime_error("Can't open file: " + filename);
    }

    //Read riff chunk
    {
        if (!this->findChunk(riffId, chunk, wavFile, isBigEndian, false))
            throw std::runtime_error(filename + " is not a WAV file");

        std::uint8_t riffType[idSize];
        wavFile.read(reinterpret_cast<char *>(&riffType), sizeof(riffType));

        if (std::memcmp(riffType, waveId, idSize) != 0)
            throw std::runtime_error("Invalid RIFF chunk in: " + filename);
    }

    //Read fmt chunk, it might not be immediately after the riff chunk
    {
        if (!this->findChunk(fmtId, chunk, wavFile, isBigEndian))
            throw std::runtime_error(filename + " is malformed");

        FmtChunk fmtChunk{};
        wavFile.read(reinterpret_cast<char *>(&fmtChunk), sizeof(fmtChunk));

        if (!checkFmtChunk(fmtChunk))
            throw std::runtime_error("WAV is not in the correct format");
    }

    std::int64_t numberOfSamples;

    //Read data chunk
    {
        if (!this->findChunk(dataId, chunk, wavFile, isBigEndian))
            throw std::runtime_error(filename + " is malformed");

        numberOfSamples = (chunk.size << 3u) / (consts::audio::CHANNELS * consts::audio::BITS_PER_SAMPLE);
    }

    //For whatever reason using a vector instead of a plain old array makes valgrind not complain
    std::vector<std::int16_t> iData;
    iData.resize(numberOfSamples); //Resize to avoid relocations
    this->data.resize(numberOfSamples);

    //Read the samples
    wavFile.read(reinterpret_cast<char *>(iData.data()), numberOfSamples * consts::audio::BITS_PER_SAMPLE >> 3u);

    if (isBigEndian)
        for (auto &sample: iData)
            sample = fin::math::integers::byteSwap(sample);

    //Convert the int16 to a float
    std::transform(iData.begin(), iData.end(), this->data.data(), [](const std::int16_t &i) -> float {
        return i;
    });

    wavFile.close();
}

const std::vector<float> &fin::readers::WavReader::getData() {
    return this->data;
}

void fin::readers::WavReader::dropSamples() {
    this->data.resize(0);
}

const std::string &fin::readers::WavReader::getBasename() {
    return basename;
}
