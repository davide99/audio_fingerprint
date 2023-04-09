#include <vector>
#include <iostream>
#include <chrono>
#include <fin_db/utils.h>
#include <fin_db/db.h>
#include <fin_db/fin_db.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Wrong number of parameters, please use: " << std::endl
                  << "-i <path to wav directory>" << std::endl
                  << "-d database => drop database" << std::endl;
    } else {
        auto command = fin::utils::trim(std::string(argv[1]));
        auto argument = fin::utils::trim(std::string(argv[2]));

        fin::DB db;

        if (command == "-i") {
#ifdef DEBUG
            db.drop();
            db.create();
#endif
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::string> fileList = fin::utils::listFiles(argument, "wav");

            for (const auto &fileName:fileList) {
                std::cout << "Analyzing " << fileName << std::endl;
                fin::insertSong(fileName, db);
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
