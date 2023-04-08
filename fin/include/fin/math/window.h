#ifndef FIN_MATH_WINDOW_H
#define FIN_MATH_WINDOW_H

#include <array>
#include <vector>
#include "../../../consts.h"

namespace fin::math::window {
    extern const std::array<float, consts::window::SIZE> WINDOW;
    extern const std::array<float, consts::window::FREQ_BINS> FREQ_BINS;
    extern const std::vector<int> BANDS;
    extern const std::array<int, consts::window::FREQ_BINS> BANDS_MAP;

    const int &getBandIndex(const int &freqIndex);
}

#endif
