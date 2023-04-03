#ifndef MATH_INTEGERS_H
#define MATH_INTEGERS_H

#include <cstdint>
#include <string>

namespace math::integers {
    std::uint64_t byte_swap(const std::uint64_t &x);

    std::uint32_t byte_swap(const std::uint32_t &x);

    std::uint16_t byte_swap(const std::uint16_t &x);

    std::int16_t byte_swap(const std::int16_t &x);

    std::int64_t byte_swap(const std::int64_t &x);
}

#endif
