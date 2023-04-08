#ifndef FIN_CORE_PEAKS_FINDER_H
#define FIN_CORE_PEAKS_FINDER_H

#include "../math/fft_window.h"
#include "../utils/fixed_size_pq.h"
#include "peak.h"
#include <vector>
#include <cstdint>

namespace fin::core {
    /**
     * Finds the peaks in the fft_window between band_start and band_end
     * @param fft_window FFT window to analyze
     * @param window    Number of the window, to be used in the peak initialization
     * @param band_start Lower boundary included
     * @param band_end   Upper boundary included
     * @return The found peaks
     */
    utils::fixed_size_pq<peak, consts::fingerprint::n_peaks>
    find_peaks(const math::fft_window &fft_window, const std::int64_t &window,
               const int &band_start, const int &band_end);
}

#endif
