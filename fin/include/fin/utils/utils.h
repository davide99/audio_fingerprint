#ifndef FIN_CPP_UTILS_H
#define FIN_CPP_UTILS_H

#include <string>
#include <vector>
#include <fin/core/links.h>
#include <fin/readers/reader.h>
#include <fin/math/spectrogram.h>
#include <fin/core/fingerprint.h>

namespace fin::utils {
    bool is_big_endian();

    /**
    * Compute links given a generic reader
    * @param reader where to read samples from
    * @return computed links
    */
    fin::core::links computeLinks(fin::readers::reader &reader);
}

#endif
