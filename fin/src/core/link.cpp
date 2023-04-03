#include "../../include/core/link.h"
#include "../../include/utils/utils.h"
#include "../../include/math/integers.h"
#include <xxh3.h>

core::link::link(const peak &address, const peak &peak) {
    std::int64_t delta_time = peak.get_window() - address.get_window();
    std::int64_t delta_freq = peak.get_freq_index() - address.get_freq_index();
    std::uint64_t addr_freq = address.get_freq_index();

    XXH3_state_t state;
    XXH3_64bits_reset(&state);

    if (utils::is_big_endian()) {
        delta_time = math::integers::byte_swap(delta_time);
        delta_freq = math::integers::byte_swap(delta_freq);
        addr_freq = math::integers::byte_swap(addr_freq);
    }

    XXH3_64bits_update(&state, &delta_time, sizeof(delta_time));
    XXH3_64bits_update(&state, &delta_freq, sizeof(delta_freq));
    XXH3_64bits_update(&state, &addr_freq, sizeof(addr_freq));

    this->hash = XXH3_64bits_digest(&state);
    this->window = address.get_window();
}

std::uint64_t core::link::get_hash() const {
    return this->hash;
}

std::uint64_t core::link::get_time() const {
    return this->window;
}

