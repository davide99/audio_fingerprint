#include "WavReader.h"
#include "../Utils/Utils.h"
#include "../Math/Integers.h"
#include "../Consts.h"
#include <iostream>
#include <fstream>
#include <cstdint>
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
    //Chunk defined above
    std::uint16_t audioFormat;
    std::uint16_t channels;
    std::uint32_t sampleRate;
    std::uint32_t averageBytePerSec;
    std::uint16_t blockAlign;
    std::uint16_t bitsPerSample;
};
#pragma pack(pop)


//Chunk IDs
constexpr std::uint8_t RiffID[] = {0x52u, 0x49u, 0x46u, 0x46u}; //RIFF
constexpr std::uint8_t WaveID[] = {0x57u, 0x41u, 0x56u, 0x45u}; //WAVE
constexpr std::uint8_t FmtID[] = {0x66u, 0x6Du, 0x74u, 0x20u};  //fmt_
constexpr std::uint8_t DataID[] = {0x64u, 0x61u, 0x74u, 0x61u}; //DATA

//Check if the WAV has the correct characteristics
static bool checkFmtChunk(const FmtChunk &fmtChunk) {
    return (fmtChunk.audioFormat == 1u) &&                  //PCM
           (fmtChunk.channels == Consts::Audio::Channels) &&
           (fmtChunk.sampleRate == Consts::Audio::SampleRate) &&
           (fmtChunk.bitsPerSample == Consts::Audio::BitsPerSample);
}

//Iterate through the WAV file to find a chunk given the id
bool IO::WavReader::findChunk(const std::uint8_t *id, Chunk &chunk, std::ifstream &wavFile, const bool &isBigEndian,
                              const bool iterate = true) {
    bool found;

    do {
        try {
            wavFile.read(reinterpret_cast<char *>(&chunk), sizeof(Chunk)); //Read a chunk
        } catch (const std::ifstream::failure &e) {
            return false;
        }

        if (isBigEndian)
            chunk.size = Math::Integers::BSwap(chunk.size);

        found = !std::memcmp(chunk.id, id, idSize); //Compare with the given id

        if (!found)
            try {
                //seek from the current position ahead of chunk.size bytes
                wavFile.seekg(chunk.size, std::ios_base::cur);
            } catch (const std::ifstream::failure &e) {
                return false;
            }

    } while (iterate && !found);

    return true;
}

IO::WavReader::WavReader(const std::string &fileName) {
    Chunk chunk{};
    bool isBigEndian;

    this->fileName = fileName;

    if (!Utils::fileExists(fileName))
        throw std::runtime_error("Can't open file: " + fileName);

    isBigEndian = Utils::isBigEndian();

    std::ifstream wavFile(fileName, std::ios::binary);
    wavFile.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);

    //Read riff chunk
    {
        if (!this->findChunk(RiffID, chunk, wavFile, isBigEndian, false))
            throw std::runtime_error(fileName + " is not a WAV file");

        std::uint8_t riffType[idSize];
        wavFile.read(reinterpret_cast<char *>(&riffType), sizeof(riffType));

        if (std::memcmp(riffType, WaveID, idSize) != 0)
            throw std::runtime_error("Invalid RIFF chunk in: " + fileName);
    }

    //Read fmt chunk, it might not be the immediately after the riff chunk
    {
        if (!this->findChunk(FmtID, chunk, wavFile, isBigEndian))
            throw std::runtime_error(fileName + " is malformed");

        FmtChunk fmtChunk{};
        wavFile.read(reinterpret_cast<char *>(&fmtChunk), sizeof(FmtChunk));

        if (!checkFmtChunk(fmtChunk))
            throw std::runtime_error("WAV is not in the correct format");
    }

    size_t numberOfSamples;

    //Read data chunk
    {
        if (!this->findChunk(DataID, chunk, wavFile, isBigEndian))
            throw std::runtime_error(fileName + " is malformed");

        numberOfSamples = (chunk.size << 3u) / (Consts::Audio::Channels * Consts::Audio::BitsPerSample);
    }

    //For whatever reason using a vector instead of a plain old array makes valgrind not complain
    std::vector<std::int16_t> iData;
    iData.resize(numberOfSamples); //Resize to avoid reallocations
    this->data.resize(numberOfSamples);

    //Read the samples
    wavFile.read(reinterpret_cast<char *>(iData.data()), numberOfSamples * Consts::Audio::BitsPerSample >> 3u);

    if (isBigEndian)
        for (auto &sample : iData)
            sample = Math::Integers::BSwap(sample);

    //Convert the int16 to a float
    std::transform(iData.begin(), iData.end(), this->data.data(), [](const std::int16_t &i) -> float {
        return i;
    });

    wavFile.close();
}

const std::vector<float> &IO::WavReader::getData() {
    return this->data;
}

const std::string &IO::WavReader::getFileName() const {
    return this->fileName;
}
