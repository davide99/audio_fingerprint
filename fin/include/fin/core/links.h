#ifndef FIN_CORE_LINKS_H
#define FIN_CORE_LINKS_H

#include <vector>
#include <fin/core/link.h>
#include <fin/core/peak.h>
#include <fin/utils/byte_buffer.h>

namespace fin::core {
    /**
     * vector of links. Access the peaks as you would normally do with a vector.
     */
    class Links : public std::vector<Link> {
    public:
        /**
         * Constructs the links given the peaks
         * @param peakVec Computed peaks
         */
        explicit Links(std::vector<Peak> &peakVec);

        fin::utils::ByteBuffer toByteBuffer();

        static fin::core::Links fromByteBuffer(utils::ByteBuffer &byteBuffer);

    private:
        Links() = default;
    };
}


#endif
