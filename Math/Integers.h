#ifndef MATH_INTEGERS_H
#define MATH_INTEGERS_H

#include <cstdint>
#include <string>

namespace Math::Integers {
    std::uint64_t BSwap(const std::uint64_t &x);

    std::uint32_t BSwap(const std::uint32_t &x);

    std::uint16_t BSwap(const std::uint16_t &x);

    std::int16_t BSwap(const std::int16_t &x);

    std::int64_t BSwap(const std::int64_t &x);

    /**
     * Efficiently "converts" a uint64 to a string_view
     * @param x uint to convert
     * @return the converted number wrapped in a string_view
     */
    std::string_view toHex(std::uint64_t x);
}

#endif
