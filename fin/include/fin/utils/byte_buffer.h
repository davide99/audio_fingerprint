#ifndef FIN_UTILS_BYTE_BUFFER_H
#define FIN_UTILS_BYTE_BUFFER_H

#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <type_traits>
#include <fin/utils/utils.h>

namespace fin::utils {
    class ByteBuffer {
    private:
        std::unique_ptr<std::uint8_t[]> data_;
        std::size_t capacity_;
        std::size_t readOffset_;
        std::size_t size_;
        bool isBigEndian_;

    public:
        explicit ByteBuffer(const std::size_t &initialCapacity = 16) :
                data_{std::make_unique<std::uint8_t[]>(initialCapacity)},
                capacity_{initialCapacity}, readOffset_{0}, size_{0},
                isBigEndian_{fin::utils::isBigEndian()} {};

        template<typename T>
        void add(const T &data) {
            auto *rawData = reinterpret_cast<const std::uint8_t *>(std::addressof(data));
            ensureAvailableCapacity(sizeof(T));

            if (isBigEndian_) {
                std::reverse_copy(rawData, rawData + sizeof(T), data_.get() + size_);
            } else {
                std::copy(rawData, rawData + sizeof(T), data_.get() + size_);
            }

            size_ += sizeof(T);
        }

        void add(const std::uint8_t *data, std::size_t len) {
            ensureAvailableCapacity(len);

            std::copy(data, data + len, data_.get() + size_);
            size_ += len;
        }

        template<typename T>
        void remove(T &data) {
            auto *rawData = reinterpret_cast<std::uint8_t *>(std::addressof(data));

            if (sizeof(T) > size_ - readOffset_)
                throw std::runtime_error("Not enough space left to read in the ByteBuffer");

            if (isBigEndian_) {
                std::reverse_copy(data_.get() + readOffset_, data_.get() + readOffset_ + sizeof(T), rawData);
            } else {
                std::copy(data_.get() + readOffset_, data_.get() + readOffset_ + sizeof(T), rawData);
            }

            readOffset_ += sizeof(T);
        }

        std::unique_ptr<std::uint8_t[]> &getPtr() {
            return data_;
        }

        [[nodiscard]] std::size_t &getSize() {
            return size_;
        }

    private:
        void ensureAvailableCapacity(const std::size_t &availableCapacity) {
            const std::size_t currentAvailableCapacity = capacity_ - size_;

            if (availableCapacity > currentAvailableCapacity)
                realloc(availableCapacity - currentAvailableCapacity);
        }

        void realloc(const std::size_t &minimum_increment) {
            std::size_t new_minimum_capacity = capacity_ + minimum_increment;
            std::size_t new_power_of_2_capacity = 1;
            while (new_power_of_2_capacity < new_minimum_capacity)
                new_power_of_2_capacity <<= 1u;

            auto new_ptr = std::make_unique<std::uint8_t[]>(new_power_of_2_capacity);
            std::copy(data_.get(), data_.get() + size_, new_ptr.get());

            data_ = std::move(new_ptr);
            capacity_ = new_power_of_2_capacity;
        }
    };
}

#endif
