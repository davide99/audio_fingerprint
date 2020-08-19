#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#include <cstdint>
#include <algorithm>
#include <functional>

namespace Math::Vector {

    /**
     * Multiply array v1 by v2 element-wise and store the output in out
     * @tparam T    type of an array element
     * @param v1    first vector
     * @param v2    second vector
     * @param out   output vector
     * @param len   length of v1, v2 and out
     */
    template<typename T>
    inline void mul(const T *v1, const T *v2, T *out, std::size_t len) {
        std::transform(v1, v1 + len, v2, out, std::multiplies<T>());
    }
}

#endif
