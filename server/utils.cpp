#include "utils.h"
#include <filesystem>
#include <algorithm>
#include <cstring>


//https://stackoverflow.com/a/2072890/6441490
bool utils::ends_with(const std::string &str, const std::string &ending) {
    if (ending.size() > str.size())
        return false;

    return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

std::vector<std::string> utils::list_files(const std::string &path, const std::string &extension) {
    namespace fs = std::filesystem;
    std::vector<std::string> files;

    for (const auto &p : fs::directory_iterator(path))
        if (!p.is_directory() && (extension.empty() || ends_with(p.path().string(), extension)))
            files.push_back(p.path().string());

    return files;
}

std::string utils::trim(const std::string &str) {
    std::string s = str;

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());

    return s;
}

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

std::string_view utils::to_hex(std::uint64_t x) {
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

    return {buff + i, buffLen - i};
}