#include <fin/utils/utils.h>
#include <cstdint>

bool fin::utils::isBigEndian() {
    uint16_t n = 0x0001u;
    auto *ptr = reinterpret_cast<uint8_t *>(&n);

    return ptr[0] == 0;
}
