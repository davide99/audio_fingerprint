#ifndef MATH_WINDOW_H
#define MATH_WINDOW_H

#include <array>
#include <vector>
#include "../../../consts.h"

namespace fin::math::window {
    extern const std::array<float, consts::window::size> window;
    extern const std::array<float, consts::window::freq_bins> freq_bins;
    extern const std::vector<int> bands;
    extern const std::array<int, consts::window::freq_bins> bands_map;

    const int &get_band_index(const int &freq_index);
}

#endif
