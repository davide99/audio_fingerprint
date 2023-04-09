#ifndef FIN_UTILS_BYTE_BUFFER_H
#define FIN_UTILS_BYTE_BUFFER_H

#include <vector>
#include <cstdint>

namespace fin::utils {
    class ByteBuffer {
    private:
        std::vector<std::uint8_t> data_;
        std::size_t read_pos_;

    public:
        ByteBuffer() : data_{}, read_pos_{0} {};

        void add8(const std::uint8_t &data);

        std::uint8_t remove8();

        void add16(const std::uint16_t &data);

        std::uint16_t remove16();

        void add32(const std::uint32_t &data);

        std::uint32_t remove32();

        void add64(const std::uint64_t &data);

        std::uint64_t remove64();

        const std::uint8_t *getData();

        std::size_t getSize();
    };
}

#endif
