#include <fin/core/peak.h>

const int &fin::core::peak::get_freq_index() const {
    return this->freq_index;
}

const std::int64_t &fin::core::peak::get_window() const {
    return this->window;
}

bool fin::core::peak::same_band(const peak &peak) const {
    return this->band_index == peak.band_index;
}

bool fin::core::peak::operator<(const peak &peak) const {
    return this->power < peak.power;
}

bool fin::core::peak::operator>(const core::peak &peak) const {
    return this->power > peak.power;
}

const float &fin::core::peak::get_time() const {
    return this->time;
}
