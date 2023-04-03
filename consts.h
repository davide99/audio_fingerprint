#ifndef CONSTS_H
#define CONSTS_H

#include <cstdint>
#include <string>
#include <array>

namespace consts {
    namespace audio {
        constexpr std::uint16_t CHANNELS = 1u;
        constexpr std::uint32_t SAMPLE_RATE = 8000u;
        constexpr std::uint16_t BITS_PER_SAMPLE = 16u;
    }

    namespace window {
        constexpr std::uint16_t size = 512u;
        constexpr std::uint16_t Overlap = 256u;
        constexpr std::uint16_t step_size = size - Overlap;
        constexpr std::uint16_t freq_bins = size / 2;

        constexpr std::uint16_t MelStart = 250u;
        constexpr std::uint16_t MelStep = 200u;

        constexpr float FreqBinStep = ((float) audio::SAMPLE_RATE) / size;
    }

    namespace fingerprint {
        constexpr std::uint8_t C = 32u;
        constexpr std::uint8_t n_peaks = 3u;
        constexpr std::uint8_t PeakRange = 5u;
        constexpr std::uint16_t MinFreq = 100u;
        constexpr std::uint16_t MaxFreq = 2000u;
    }

    namespace Links {
        constexpr float MinWinDistanceF = 1.0f;
        constexpr float MaxWinDistanceF = 3.0f;

        constexpr std::uint64_t MinHint = 15;
    }

    namespace db {
        //Credentials
        const std::string HOSTNAME = "localhost";
        const std::string USERNAME = "root";
        const std::string PASSWORD = "davide";
        const std::string NAME = "musiclibrary";

        //Table names
        const std::string RECORDINGS_TABLE = "recordings";
        const std::string INFO_TABLE = "musicinfo";
        const std::string TMP_RECORD_TABLE = "_recording";

        const std::string RECORDINGS_TABLE_FULL = NAME + "." + RECORDINGS_TABLE;
        const std::string INFO_TABLE_FULL = NAME + "." + INFO_TABLE;
        const std::string TMP_RECORD_TABLE_FULL = NAME + "." + TMP_RECORD_TABLE;

        //Table fields
        const std::string UInt64 = "BIGINT UNSIGNED";
        const std::string UInt = "INT UNSIGNED";
    }
}

#endif
