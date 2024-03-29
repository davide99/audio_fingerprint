#ifndef FIN_CORE_LINK_H
#define FIN_CORE_LINK_H

#include <cstdint>
#include <fin/core/peak.h>

namespace fin::core {
    /**
     * Class to hold a link information
     */
    class Link {
    private:
        std::uint64_t hash_;
        std::uint64_t window_;
    public:
        Link(const fin::core::Peak &address, const fin::core::Peak &peak);

        Link(const std::uint64_t &hash, const std::uint64_t &window);

        [[nodiscard]] const std::uint64_t &getHash() const;

        [[nodiscard]] const std::uint64_t &getTime() const;
    };
}

#endif
