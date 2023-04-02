#include "Integers.h"
#include <algorithm>
#include <cstring>

std::uint32_t Math::Integers::BSwap(const std::uint32_t &x) {
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

std::uint16_t Math::Integers::BSwap(const std::uint16_t &x) {
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

std::int16_t Math::Integers::BSwap(const std::int16_t &x) {
    std::uint16_t val_u = *(std::uint16_t *) &x;  //Convert to unsigned
    val_u = BSwap(val_u);                         //Convert to big endian
    return *(std::int16_t *) &val_u;              //Make it signed
}

std::uint64_t Math::Integers::BSwap(const std::uint64_t &x) {
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

std::int64_t Math::Integers::BSwap(const int64_t &x) {
    std::uint64_t val_u = *(std::uint64_t *) &x;  //Convert to unsigned
    val_u = BSwap(val_u);                         //Convert to big endian
    return *(std::int64_t *) &val_u;              //Make it signed
}


#ifdef BUILD_SERVER
static constexpr auto MaxUInt64Len = 16u;
static constexpr auto buffLen = MaxUInt64Len + 2u;
static thread_local char buff[buffLen];
static constexpr char hexPre[] = "0x";
static constexpr char hexTable[] = "000102030405060708090A0B0C0D0E0F"
                                   "101112131415161718191A1B1C1D1E1F"
                                   "202122232425262728292A2B2C2D2E2F"
                                   "303132333435363738393A3B3C3D3E3F"
                                   "404142434445464748494A4B4C4D4E4F"
                                   "505152535455565758595A5B5C5D5E5F"
                                   "606162636465666768696A6B6C6D6E6F"
                                   "707172737475767778797A7B7C7D7E7F"
                                   "808182838485868788898A8B8C8D8E8F"
                                   "909192939495969798999A9B9C9D9E9F"
                                   "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
                                   "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
                                   "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
                                   "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
                                   "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
                                   "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

std::string_view Math::Integers::toHex(std::uint64_t x) {
    int i;
    std::uint_fast8_t byte;

    i = buffLen; //Start from the MSB
    do {
        i -= 2;
        byte = x & 0xFFu;
        std::memcpy(buff + i, hexTable + (byte << 1u), 2); //C vibes

        x >>= 8u; //Eats away a byte
    } while (x != 0);

    if (buff[i] == '0') //we got 0X'XX'...?
        i++;

    i -= 2;

    std::memcpy(buff + i, hexPre, 2); //Copy the "0x"

    return std::string_view(buff + i, buffLen - i);
}
#endif
