#include "../../include/core/peak.h"

const int &core::peak::get_freq_index() const {
    return this->freq_index;
}

const std::int64_t & core::peak::get_window() const {
    return this->window;
}

bool core::peak::same_band(const peak &peak) const {
    return this->band_index == peak.band_index;
}

bool core::peak::operator<(const peak &peak) const {
    return this->power < peak.power;
}

bool core::peak::operator>(const core::peak &peak) const {
    return this->power > peak.power;
}

const float &core::peak::get_time() const {
    return this->time;
}
