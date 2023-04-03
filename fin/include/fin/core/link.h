#ifndef CORE_LINK_H
#define CORE_LINK_H

#include <cstdint>
#include <fin/core/peak.h>

namespace fin::core {
    /**
     * Class to hold a link information
     */
    class link {
    private:
        std::uint64_t hash;
        std::uint64_t window;
    public:
        link(const fin::core::peak &address, const fin::core::peak &peak);

        [[nodiscard]] std::uint64_t get_hash() const;

        [[nodiscard]] std::uint64_t get_time() const;
    };
}


#endif
