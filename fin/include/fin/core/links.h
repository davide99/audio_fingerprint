#ifndef CORE_LINKS_H
#define CORE_LINKS_H

#include <vector>
#include <fin/core/link.h>
#include <fin/core/peak.h>

namespace fin::core {
    /**
     * vector of links. Access the peaks as you would normally do with a vector.
     */
    class links : public std::vector<link> {
    public:
        /**
         * Constructs the links given the peaks
         * @param peak_vec Computed peaks
         */
        explicit links(const std::vector<fin::core::peak> &peak_vec);
    };
}


#endif
