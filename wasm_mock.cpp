#include <string>
#include <iostream>
#include "IO/Readers/WavReader.h"
#include "Utils/Wrapper.h"

int main(int argc, char **argv) {
    if (argc != 2) return -1;

    IO::Readers::WavReader reader(argv[1]);
    auto links = Utils::computeLinks(reader);

    std::string json = "[";
    for (auto &link: links) {
        json += "{\"hash\":" + std::to_string(link.getHash()) +
                ", \"window\":" + std::to_string(link.getTime()) + "},";
    }
    json.pop_back();
    json += "]";

    std::cout << json << std::endl;
    return 0;
}
