#ifndef FIN_CORE_PEAK_H
#define FIN_CORE_PEAK_H

#include <cstdint>
#include <type_traits>
#include <fin/math/window.h>
#include "../../../consts.h"

namespace fin::core {
    /**
     * Class to hold peak information
     */
    class Peak {
    private:
        int freqIndex_;       //index referring to an element of window::FREQ_BINS
        float power_;         //magnitude
        std::int64_t window_; //window the peak belongs to
        int bandIndex_;       //index referring to an element of window::BANDS (getBandsIndex)

    public:
        Peak(const int &freqIndex, const float &power, const std::int64_t &window) :
                freqIndex_(freqIndex), power_(power), window_(window),
                bandIndex_(math::window::getBandIndex(freqIndex)) {};

        [[nodiscard]] const int &getFreqIndex() const;

        [[nodiscard]] const std::int64_t &getWindow() const;

        /**
         * Check whether the current peak and the other belong to
         * the same band
         * @param peak Other peak to compare
         * @return true if in the same band, false otherwise
         */
        [[nodiscard]] bool sameBand(const Peak &peak) const;

        /**
         * Operator to decreasingly sort peaks by power
         * @param peak Other peak
         * @return true if this.power < other.power, false otherwise
         */
        bool operator<(const Peak &peak) const;

        /**
         * Operator to decreasingly sort peaks by power
         * @param peak Other peak
         * @return true if this.power > other.power, false otherwise
         */
        bool operator>(const Peak &peak) const;

    };
}

#endif
