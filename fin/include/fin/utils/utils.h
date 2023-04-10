#ifndef FIN_UTILS_UTILS_H
#define FIN_UTILS_UTILS_H

#include <string>
#include <vector>

namespace fin::utils {
    bool isBigEndian();

    std::string getBasename(const std::string &path);
}

#endif
