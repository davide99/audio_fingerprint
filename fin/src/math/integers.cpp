#include <fin/math/integers.h>
#include <algorithm>

std::uint32_t fin::math::integers::byte_swap(const std::uint32_t &x) {
#if defined(_MSC_VER)
    return _byteswap_ulong((x));
#elif (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
    return __builtin_bswap32((x));
#elif defined(__has_builtin) && __has_builtin(__builtin_bswap64)
    return __builtin_bswap32((x));
#else
    return ((( x & 0xff000000u ) >> 24u ) |
            (( x & 0x00ff0000u ) >> 8u  ) |
            (( x & 0x0000ff00u ) << 8u  ) |
            (( x & 0x000000ffu ) << 24u ));
#endif
}

std::uint16_t fin::math::integers::byte_swap(const std::uint16_t &x) {
#if defined(_MSC_VER)
    return _byteswap_ushort((x));
#elif (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
    return __builtin_bswap16((x));
#elif defined(__has_builtin) && __has_builtin(__builtin_bswap64)
    return __builtin_bswap16((x));
#else
    return ((( x >> 8u ) & 0xffu ) | (( x & 0xffu ) << 8u ));
#endif
}

std::int16_t fin::math::integers::byte_swap(const std::int16_t &x) {
    std::uint16_t val_u = *(std::uint16_t *) &x;  //Convert to unsigned
    val_u = byte_swap(val_u);                         //Convert to big endian
    return *(std::int16_t *) &val_u;              //Make it signed
}

std::uint64_t fin::math::integers::byte_swap(const std::uint64_t &x) {
#if defined(_MSC_VER)
    return _byteswap_uint64((x));
#elif (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
    return __builtin_bswap64((x));
#elif defined(__has_builtin) && __has_builtin(__builtin_bswap64)
    return __builtin_bswap64((x));
#else
    return ((( x & 0xff00000000000000ull ) >> 56u ) |
            (( x & 0x00ff000000000000ull ) >> 40u ) |
            (( x & 0x0000ff0000000000ull ) >> 24u ) |
            (( x & 0x000000ff00000000ull ) >> 8u  ) |
            (( x & 0x00000000ff000000ull ) << 8u  ) |
            (( x & 0x0000000000ff0000ull ) << 24u ) |
            (( x & 0x000000000000ff00ull ) << 40u ) |
            (( x & 0x00000000000000ffull ) << 56u ));
#endif
}

std::int64_t fin::math::integers::byte_swap(const int64_t &x) {
    std::uint64_t val_u = *(std::uint64_t *) &x;  //Convert to unsigned
    val_u = byte_swap(val_u);                         //Convert to big endian
    return *(std::int64_t *) &val_u;              //Make it signed
}
