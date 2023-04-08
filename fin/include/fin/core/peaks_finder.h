#ifndef FIN_CORE_PEAKS_FINDER_H
#define FIN_CORE_PEAKS_FINDER_H

#include "../math/fft_window.h"
#include "../utils/fixed_size_pq.h"
#include "peak.h"
#include <vector>
#include <cstdint>

namespace fin::core {
    /**
     * Finds the peaks in the fftWindow between bandStart and bandEnd
     * @param fftWindow FFT window to analyze
     * @param window    Number of the window, to be used in the peak initialization
     * @param bandStart Lower boundary included
     * @param bandEnd   Upper boundary included
     * @return The found peaks
     */
    utils::FixedSizePQ<Peak, consts::fingerprint::N_PEAKS>
    findPeaks(const math::FFTWindow &fftWindow, const std::int64_t &window,
              const int &bandStart, const int &bandEnd);
}

#endif
