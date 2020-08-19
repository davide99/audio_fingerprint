#include "Utils.h"
#include <unistd.h>
#include <cstdint>
#include <filesystem>
#include <algorithm>

bool Utils::fileExists(const std::string &fileName) {
    return access(fileName.c_str(), F_OK) == 0;
}

bool Utils::isBigEndian() {
    uint16_t n = 0x0001u;
    auto *ptr = reinterpret_cast<uint8_t *>(&n);

    return ptr[0] == 0;
}

//https://stackoverflow.com/a/2072890/6441490
bool Utils::endsWith(const std::string &str, const std::string &ending) {
    if (ending.size() > str.size())
        return false;

    return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

std::vector<std::string> Utils::listFiles(const std::string &path, const std::string &extension) {
    namespace fs = std::filesystem;
    std::vector<std::string> files;

    for (const auto &p : fs::directory_iterator(path))
        if (!p.is_directory() && (extension.empty() || endsWith(p.path().string(), extension)))
            files.push_back(p.path().string());

    return files;
}

std::string Utils::trim(const std::string &str) {
    std::string s = str;

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());


    return s;
}
