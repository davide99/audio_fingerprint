#ifndef FIXEDSIZEPQ_H
#define FIXEDSIZEPQ_H

#include <queue>
#include <cstdint>

namespace fin::utils {
    template<typename T, std::size_t size>
    class fixed_size_pq : private std::priority_queue<T, std::deque<T>, std::greater<T>> {
    private:
        using super = std::priority_queue<T, std::deque<T>, std::greater<T>>;

    public:
        void insert(const T &value) {
            if (super::c.size() == size && value < super::c.front())
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
