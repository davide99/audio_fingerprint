#include <vector>
#include <iostream>
#include <chrono>
#include "Utils/Utils.h"
#include "Utils/Wrapper.h"
#include "Math/Integers.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Wrong number of parameters, please use: " << std::endl
                  << "-i <path to wav directory>" << std::endl
                  << "-d database => drop database" << std::endl;
    } else {
        auto command = Utils::trim(std::string(argv[1]));
        auto argument = Utils::trim(std::string(argv[2]));

        IO::DB db;

        if (command == "-i") {
#ifdef DEBUG
            db.drop();
            db.create();
#endif
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::string> fileList = Utils::listFiles(argument, "wav");

            for (const auto &fileName:fileList) {
                std::cout << "Analyzing " << fileName << std::endl;
                Utils::insertSong(fileName, db);
            }

            if (!fileList.empty()) {
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::high_resolution_clock::now() - start).count() / fileList.size();

                std::cout << "Mean time for each song: " << duration << "ms" << std::endl;
            }
        } else if (command == "-d" && argument == "database") {
            if (db.drop())
                std::cout << "Database dropped" << std::endl;
            else
                std::cout << "Can't drop database" << std::endl;
        }
    }

    return 0;
}
