#include <fin/utils/byte_buffer.h>
#include <fin/math/integers.h>
#include <fin/utils/utils.h>

void fin::utils::ByteBuffer::add8(const std::uint8_t &data) {
    data_.push_back(data);
}

void fin::utils::ByteBuffer::add16(const std::uint16_t &data) {
    std::uint16_t tmp;

    if (fin::utils::isBigEndian()) {
        tmp = fin::math::integers::byteSwap(data);
    } else {
        tmp = data;
    }

    data_.push_back(tmp & 0xFFu);
    data_.push_back(tmp >> 8u);
}

void fin::utils::ByteBuffer::add32(const std::uint32_t &data) {
    std::uint32_t tmp;

    if (fin::utils::isBigEndian()) {
        tmp = fin::math::integers::byteSwap(data);
    } else {
        tmp = data;
    }

    data_.push_back(tmp & 0xFFu);
    data_.push_back((tmp >> 8u) & 0xFFu);
    data_.push_back((tmp >> 16u) & 0xFFu);
    data_.push_back((tmp >> 24u) & 0xFFu);
}

void fin::utils::ByteBuffer::add64(const std::uint64_t &data) {
    std::uint64_t tmp;

    if (fin::utils::isBigEndian()) {
        tmp = fin::math::integers::byteSwap(data);
    } else {
        tmp = data;
    }

    data_.push_back(tmp & 0xFFu);
    data_.push_back((tmp >> 8u) & 0xFFu);
    data_.push_back((tmp >> 16u) & 0xFFu);
    data_.push_back((tmp >> 24u) & 0xFFu);
    data_.push_back((tmp >> 32u) & 0xFFu);
    data_.push_back((tmp >> 40u) & 0xFFu);
    data_.push_back((tmp >> 48u) & 0xFFu);
    data_.push_back((tmp >> 56u) & 0xFFu);
}

const std::uint8_t *fin::utils::ByteBuffer::getData() {
    return data_.data();
}

std::size_t fin::utils::ByteBuffer::getSize() {
    return data_.size();
}

std::uint8_t fin::utils::ByteBuffer::remove8() {
    return data_[read_pos_++];
}

std::uint16_t fin::utils::ByteBuffer::remove16() {
    const std::uint16_t value =
            static_cast<std::uint16_t>(data_[read_pos_]) &
            static_cast<std::uint16_t>(data_[read_pos_ + 1]) << 8u;
    read_pos_ += 2;
    return value;
}

std::uint32_t fin::utils::ByteBuffer::remove32() {
    const std::uint32_t value =
            static_cast<std::uint32_t>(data_[read_pos_]) &
            static_cast<std::uint32_t>(data_[read_pos_ + 1]) << 8u &
            static_cast<std::uint32_t>(data_[read_pos_ + 2]) << 16u &
            static_cast<std::uint32_t>(data_[read_pos_ + 3]) << 24u;
    read_pos_ += 4;
    return value;
}

std::uint64_t fin::utils::ByteBuffer::remove64() {
    const std::uint64_t value =
            static_cast<std::uint64_t>(data_[read_pos_]) &
            static_cast<std::uint64_t>(data_[read_pos_ + 1]) << 8u &
            static_cast<std::uint64_t>(data_[read_pos_ + 2]) << 16u &
            static_cast<std::uint64_t>(data_[read_pos_ + 3]) << 24u &
            static_cast<std::uint64_t>(data_[read_pos_ + 4]) << 32u &
            static_cast<std::uint64_t>(data_[read_pos_ + 5]) << 40u &
            static_cast<std::uint64_t>(data_[read_pos_ + 6]) << 48u &
            static_cast<std::uint64_t>(data_[read_pos_ + 7]) << 56u;
    read_pos_ += 8;
    return value;
}
