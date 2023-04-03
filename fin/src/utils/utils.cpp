#include "../../include/utils/utils.h"
#include <cstdint>

bool utils::is_big_endian() {
    uint16_t n = 0x0001u;
    auto *ptr = reinterpret_cast<uint8_t *>(&n);

    return ptr[0] == 0;
}
