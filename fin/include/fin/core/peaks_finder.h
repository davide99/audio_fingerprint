#ifndef FIN_CORE_PEAKS_FINDER_H
#define FIN_CORE_PEAKS_FINDER_H

#include <fin/core/peak.h>
#include <fin/math/spectrogram.h>
#include <vector>
#include <cstdint>

namespace fin::core {
    /**
     * Given the spectrogram, computes a vector of peaks with
     * the most important (loudest) peaks of the song in it.
     * @param spectrogram Computed spectrum
     * @return vector of peaks
     */
    [[nodiscard]] std::vector<fin::core::Peak> findPeaks(const fin::math::Spectrogram &spectrogram);
}

#endif
