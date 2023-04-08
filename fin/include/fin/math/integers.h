#ifndef FIN_MATH_INTEGERS_H
#define FIN_MATH_INTEGERS_H

#include <cstdint>
#include <string>

namespace fin::math::integers {
    std::uint64_t byteSwap(const std::uint64_t &x);

    std::uint32_t byteSwap(const std::uint32_t &x);

    std::uint16_t byteSwap(const std::uint16_t &x);

    std::int16_t byteSwap(const std::int16_t &x);

    std::int64_t byteSwap(const std::int64_t &x);
}

#endif
