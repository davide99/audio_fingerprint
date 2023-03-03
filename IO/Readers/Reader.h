#ifndef GENERICREADER_H
#define GENERICREADER_H

#include <vector>

namespace IO::Readers {
    class Reader {
    public:
        virtual const std::vector<float> &getData() = 0;
    };
}

#endif
