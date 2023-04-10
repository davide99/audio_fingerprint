#include <fin/utils/utils.h>
#include <cstdint>
#include <regex>

bool fin::utils::isBigEndian() {
    uint16_t n = 0x0001u;
    auto *ptr = reinterpret_cast<uint8_t *>(&n);

    return ptr[0] == 0;
}

std::string fin::utils::getBasename(const std::string &path) {
    auto last_backslash_pos = path.find_last_of('\\');
    auto last_slash_pos = path.find_last_of('/');

    auto start = last_backslash_pos == std::string::npos ? last_slash_pos : last_backslash_pos;

    if (start == std::string::npos)
        return "";

    return path.substr(start + 1);
}
