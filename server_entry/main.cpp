#include <iostream>
#include <chrono>
#include <fin_db/utils.h>
#include <fin_db/db.h>
#include <fin_db/fin_db.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Wrong number of parameters, please use: " << std::endl
                  << argv[0] << " <path to wav directory>" << std::endl;

        return -1;
    }

    fin::DB db;
    auto input_path = fin::utils::trim(std::string(argv[1]));
    auto fileList = fin::utils::listFiles(input_path, "wav");

    if (fileList.empty()) {
        std::cerr << "No songs found in " << input_path << std::endl;
        return -1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &filename: fileList) {
        std::cout << "Analyzing " << filename << std::endl;
        fin::insertSong(filename, db);
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count() / fileList.size();

    std::cout << "Mean time for each song: " << duration << "ms" << std::endl;

    return 0;
}
