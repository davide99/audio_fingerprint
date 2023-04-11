#include <fin/core/peak.h>

const int &fin::core::Peak::getFreqIndex() const {
    return freqIndex_;
}

const std::int64_t &fin::core::Peak::getWindow() const {
    return window_;
}

bool fin::core::Peak::sameBand(const Peak &peak) const {
    return bandIndex_ == peak.bandIndex_;
}

bool fin::core::Peak::operator<(const Peak &peak) const {
    return power_ < peak.power_;
}

bool fin::core::Peak::operator>(const core::Peak &peak) const {
    return power_ > peak.power_;
}

const float &fin::core::Peak::getTime() const {
    return time_;
}
