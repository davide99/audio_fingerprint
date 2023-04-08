#ifndef FIN_CORE_LINKS_H
#define FIN_CORE_LINKS_H

#include <vector>
#include <fin/core/link.h>
#include <fin/core/peak.h>

namespace fin::core {
    /**
     * vector of links. Access the peaks as you would normally do with a vector.
     */
    class Links : public std::vector<Link> {
    public:
        /**
         * Constructs the links given the peaks
         * @param peakVec Computed peaks
         */
        explicit Links(const std::vector<fin::core::Peak> &peakVec);
    };
}


#endif
