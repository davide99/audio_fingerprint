#ifndef CORE_LINK_H
#define CORE_LINK_H

#include <cstdint>
#include "Peak.h"

namespace Core {
    /**
     * Class to hold a link information
     */
    class Link {
    private:
        std::uint64_t hash;
        std::uint64_t window;
    public:
        Link(const Peak &address, const Peak &peak);

        [[nodiscard]] std::uint64_t getHash() const;

        [[nodiscard]] std::uint64_t getTime() const;
    };
}


#endif
