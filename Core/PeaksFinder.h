#ifndef CORE_PEAKSFINDER_H
#define CORE_PEAKSFINDER_H

#include "../Math/FFTWindow.h"
#include "../Utils/FixedSizePQ.h"
#include "Peak.h"
#include <vector>
#include <cstdint>

namespace Core {
    /**
     * Finds the peaks in the fftWindow between bandStart and bandEnd
     * @param fftWindow FFT window to analyze
     * @param window    Number of the window, to be used in the peak initialization
     * @param bandStart Lower boundary included
     * @param bandEnd   Upper boundary included
     * @return The found peaks
     */
    Utils::FixedSizePQ<Peak, Consts::Fingerprint::NPeaks>
    findPeaks(const Math::FFTWindow &fftWindow, const std::size_t &window, const int &bandStart, const int &bandEnd);
}

#endif
