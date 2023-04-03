#ifndef CORE_FINGERPRINT_H
#define CORE_FINGERPRINT_H

#include <vector>
#include "peak.h"
#include "../math/spectrogram.h"

namespace core::fingerprint {
    /**
     * Given the spectrogram, computes the fingerprint i.e. a vector of peaks with
     * the most important (loudest) peaks of the song in it.
     * @param spectrogram Computed spectrum
     * @return vector of peaks
     */
    [[nodiscard]] std::vector<core::peak> compute(const math::spectrogram& spectrogram);
}


#endif
