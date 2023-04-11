#include <fin/utils/utils.h>
#include <cstdint>
#include <regex>

bool fin::utils::isBigEndian() {
    uint16_t n = 0x0001u;
    auto *ptr = reinterpret_cast<uint8_t *>(&n);

    return ptr[0] == 0;
}

std::string fin::utils::getBasename(const std::string &path) {
    auto lastBackslashPos = path.find_last_of('\\');
    auto lastSlashPos = path.find_last_of('/');

    auto start = lastBackslashPos == std::string::npos ? lastSlashPos : lastBackslashPos;

    if (start == std::string::npos)
        return "";

    return path.substr(start + 1);
}
