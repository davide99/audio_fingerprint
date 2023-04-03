#include <string>
#include <iostream>
#include "wav_reader.h"
#include "wrapper.h"

int main(int argc, char **argv) {
    if (argc != 2) return -1;

    wav_reader reader(argv[1]);
    auto links = utils::computeLinks(reader);
    reader.drop_samples();

    std::string json = "[";
    for (auto &link: links) {
        json += "{\"hash\":" + std::to_string(link.get_hash()) +
                ",\"window\":" + std::to_string(link.get_time()) + "},";
    }
    json.pop_back();
    json += "]";

    std::cout << json << std::endl;
    return 0;
}
