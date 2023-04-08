#include <fin/core/peak.h>

const int &fin::core::Peak::getFreqIndex() const {
    return this->freqIndex;
}

const std::int64_t &fin::core::Peak::getWindow() const {
    return this->window;
}

bool fin::core::Peak::sameBand(const Peak &peak) const {
    return this->bandIndex == peak.bandIndex;
}

bool fin::core::Peak::operator<(const Peak &peak) const {
    return this->power < peak.power;
}

bool fin::core::Peak::operator>(const core::Peak &peak) const {
    return this->power > peak.power;
}

const float &fin::core::Peak::getTime() const {
    return this->time;
}
