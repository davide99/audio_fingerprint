#ifndef FIN_CORE_FINGERPRINT_H
#define FIN_CORE_FINGERPRINT_H

#include <vector>
#include <fin/core/peak.h>
#include <fin/math/spectrogram.h>

namespace fin::core::fingerprint {
    /**
     * Given the spectrogram, computes the fingerprint i.e. a vector of peaks with
     * the most important (loudest) peaks of the song in it.
     * @param spectrogram Computed spectrum
     * @return vector of peaks
     */
    [[nodiscard]] std::vector<fin::core::Peak> compute(const fin::math::Spectrogram& spectrogram);
}


#endif
