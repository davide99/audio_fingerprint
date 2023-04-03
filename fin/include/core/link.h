#ifndef CORE_LINK_H
#define CORE_LINK_H

#include <cstdint>
#include "peak.h"

namespace core {
    /**
     * Class to hold a link information
     */
    class link {
    private:
        std::uint64_t hash;
        std::uint64_t window;
    public:
        link(const peak &address, const peak &peak);

        [[nodiscard]] std::uint64_t get_hash() const;

        [[nodiscard]] std::uint64_t get_time() const;
    };
}


#endif
