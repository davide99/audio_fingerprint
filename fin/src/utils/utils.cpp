#include <fin/utils/utils.h>
#include <cstdint>
#include <regex>

bool fin::utils::isBigEndian() {
    uint16_t n = 0x0001u;
    auto *ptr = reinterpret_cast<uint8_t *>(&n);

    return ptr[0] == 0;
}

std::string fin::utils::getBasename(const std::string &path) {
    std::regex regex(R"([/\\]([^/\\]*)\.wav$)", std::regex_constants::icase);
    std::smatch matches;

    if (std::regex_search(path, matches, regex) && matches.size() == 2) {
        return matches[1].str();
    }

    return "";
}
