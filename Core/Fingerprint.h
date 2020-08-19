#ifndef CORE_FINGERPRINT_H
#define CORE_FINGERPRINT_H

#include <vector>
#include "Peak.h"
#include "../Math/Spectrogram.h"

namespace Core::Fingerprint {
    /**
     * Given the spectrogram, computes the fingerprint i.e. a vector of peaks with
     * the most important (loudest) peaks of the song in it.
     * @param spectrogram Computed spectrum
     * @return Vector of peaks
     */
    [[nodiscard]] std::vector<Core::Peak> compute(const Math::Spectrogram& spectrogram);
}


#endif
