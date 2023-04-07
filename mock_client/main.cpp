#include <string>
#include <iostream>
#include <fin/readers/wav_reader.h>
#include <fin/utils/utils.h>

int main(int argc, char **argv) {
    if (argc != 2) return -1;

    fin::readers::wav_reader reader(argv[1]);
    auto links = fin::utils::computeLinks(reader);
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
