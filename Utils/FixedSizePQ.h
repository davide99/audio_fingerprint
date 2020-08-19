#ifndef FIXEDSIZEPQ_H
#define FIXEDSIZEPQ_H

#include <queue>
#include <cstdint>

namespace Utils {
    template<typename T, std::size_t size>
    class FixedSizePQ : private std::priority_queue<T, std::deque<T>, std::greater<T>> {
    private:
        using super = std::priority_queue<T, std::deque<T>, std::greater<T>>;

    public:
        void insert(const T &value) {
            if (value < super::c.front() && super::c.size() == size)
                return;

            super::push(value);

            if (super::c.size() > size)
                super::c.pop_front();
        }

        typename std::deque<T>::reverse_iterator begin() {
            return super::c.rbegin();
        }

        typename std::deque<T>::reverse_iterator end() {
            return super::c.rend();
        }

        void clear() {
            super::c.clear();
        }
    };
}

#endif
