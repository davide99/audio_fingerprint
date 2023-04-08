#ifndef FIN_UTILS_UTILS_H
#define FIN_UTILS_UTILS_H

#include <string>
#include <vector>
#include <fin/core/links.h>
#include <fin/readers/reader.h>

namespace fin::utils {
    bool isBigEndian();

    /**
    * Compute links given a generic reader
    * @param reader where to read samples from
    * @return computed links
    */
    fin::core::Links computeLinks(fin::readers::Reader &reader);
}

#endif
