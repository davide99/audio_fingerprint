#ifndef CORE_LINKS_H
#define CORE_LINKS_H

#include <vector>
#include "Link.h"
#include "Peak.h"

namespace Core {
    /**
     * Vector of links. Access the peaks as you would normally do with a vector.
     */
    class Links : public std::vector<Link> {
    public:
        /**
         * Constructs the links given the peaks
         * @param peakVec Computed peaks
         */
        explicit Links(const std::vector<Peak> &peakVec);
    };
}


#endif
