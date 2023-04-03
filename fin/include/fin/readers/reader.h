#ifndef GENERICREADER_H
#define GENERICREADER_H

#include <vector>

namespace fin::readers {
    class reader {
    public:
        virtual const std::vector<float> &get_data() = 0;
        virtual void drop_samples() = 0;
    };
}

#endif
