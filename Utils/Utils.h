#ifndef FIN_CPP_UTILS_H
#define FIN_CPP_UTILS_H

#include <string>
#include <vector>

namespace Utils {
    bool isBigEndian();

#ifdef BUILD_SERVER
    /**
     * @param str haystack
     * @param ending needle
     * @return true if str ends with ending, false otherwise
     */
    bool endsWith(const std::string &str, const std::string &ending);

    /**
     * @param path      to search for files
     * @param extension extension of the files to check for
     * @return          vector of found files
     */
    std::vector<std::string> listFiles(const std::string &path, const std::string &extension = "");

    std::string trim(const std::string &str);
#endif
}

#endif
