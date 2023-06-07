#include <string>
#include <iostream>
#include <fin/readers/wav_reader.h>
#include <fin/fin.h>
#include <fin_db/fin_db.h>
#include <fin_db/db.h>

//#define TO_FILE

int main(int argc, char **argv) {
    if (argc != 2) return -1;

    fin::readers::WavReader reader(argv[1]);
    auto links = fin::computeLinks(reader);
    reader.dropSamples();

    fin::utils::ByteBuffer buf = links.toByteBuffer();

#ifdef TO_FILE
    FILE* pFile;
    pFile = fopen("file.binary", "wb");
    fwrite(buf.getData(), 1, buf.getSize(), pFile);
    fclose(pFile);
#endif

    auto links_out = fin::core::Links::fromByteBuffer(buf);

    fin::DB db;
    auto result = fin::searchFromLinks(links, db);

    if (result.found) {
        std::cout << result.name << std::endl;
        return 0;
    } else {
        return -1;
    }
}
