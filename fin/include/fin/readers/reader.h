#ifndef FIN_READERS_READER_H
#define FIN_READERS_READER_H

#include <vector>

namespace fin::readers {
    class Reader {
    public:
        virtual const std::vector<float> &getData() = 0;
        virtual void dropSamples() = 0;
    };
}

#endif
