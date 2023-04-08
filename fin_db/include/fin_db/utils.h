#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <string>
#include <vector>

namespace fin::utils {
    bool ends_with(const std::string &str, const std::string &ending);

    /**
     * @param path      to search for files
     * @param extension extension of the files to check for
     * @return          vector of found files
     */
    std::vector<std::string> list_files(const std::string &path, const std::string &extension = "");

    std::string trim(const std::string &str);

    std::string_view to_hex(std::uint64_t x);
}

#endif
