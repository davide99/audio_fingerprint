#ifndef UTILS_WRAPPER_H
#define UTILS_WRAPPER_H

#include <string>
#include <fin/math/spectrogram.h>
#include <fin/core/fingerprint.h>
#include <fin/core/links.h>
#include <fin/readers/reader.h>

/*
 * Just a wrapper around the two main functions
 */
namespace utils {
    /**
    * Compute links given a generic reader
    * @param reader where to read samples from
    * @return computed links
    */
    fin::core::links computeLinks(fin::readers::reader &reader) {
        fin::math::spectrogram spectrogram(reader.get_data());
        std::vector<fin::core::peak> peaks = fin::core::fingerprint::compute(spectrogram);
        return fin::core::links(peaks);
    }
}

#endif