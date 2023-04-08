#ifndef FIN_FIN_H
#define FIN_FIN_H

#include <fin/core/links.h>
#include <fin/readers/reader.h>

namespace fin {
    /**
    * Compute links given a generic reader
    * @param reader where to read samples from
    * @return computed links
    */
    fin::core::Links computeLinks(fin::readers::Reader &reader);
}

#endif
