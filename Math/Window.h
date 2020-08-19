#ifndef MATH_WINDOW_H
#define MATH_WINDOW_H

#include "../Consts.h"
#include <array>
#include <vector>

namespace Math::Window {
    extern const std::array<float, Consts::Window::Size> window;
    extern const std::array<float, Consts::Window::FreqBins> freqBins;
    extern const std::vector<int> bands;
    extern const std::array<int, Consts::Window::FreqBins> bandsMap;

    const int &getBandIndex(const int &freqIndex);
}

#endif
