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
        constexpr std::uint16_t SIZE = 512u;
        constexpr std::uint16_t OVERLAP = 256u;
        constexpr std::uint16_t STEP_SIZE = SIZE - OVERLAP;
        constexpr std::uint16_t FREQ_BINS = SIZE / 2;

        constexpr std::uint16_t MEL_START = 250u;
        constexpr std::uint16_t MEL_STEP = 200u;

        constexpr float FREQ_BIN_STEP = ((float) audio::SAMPLE_RATE) / SIZE;
    }

    namespace fingerprint {
        constexpr std::uint8_t C = 32u;
        constexpr std::uint8_t N_PEAKS = 3u;
        constexpr std::uint8_t PEAK_RANGE = 5u;
        constexpr std::uint16_t MIN_FREQ = 100u;
        constexpr std::uint16_t MAX_FREQ = 2000u;
    }

    namespace links {
        constexpr float MIN_WIN_DISTANCE_F = 1.0f;
        constexpr float MAX_WIN_DISTANCE_F = 3.0f;

        constexpr std::uint64_t MIN_HINT = 15;
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
        const std::string UINT64 = "BIGINT UNSIGNED";
        const std::string UINT = "INT UNSIGNED";
    }
}

#endif
