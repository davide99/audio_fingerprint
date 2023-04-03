#ifndef STRUCTURES_PEAK_H
#define STRUCTURES_PEAK_H

#include <cstdint>
#include <type_traits>
#include "../math/window.h"
#include "../../../consts.h"

namespace fin::core {
    /**
     * Class to hold peak information
     */
    class peak {
    private:
        int freq_index;       //index referring to an element of window::freq_bins
        float power;         //magnitude
        std::int64_t window; //window the peak belongs to
        float time;          //time in the audio file, related to the window member
        int band_index;       //index referring to an element of window::bands (getBandsIndex)

    public:
        peak(const int &freq_index, const float &power, const std::int64_t &window, const float &time) :
                freq_index(freq_index), power(power), window(window), time(time),
                band_index(math::window::get_band_index(freq_index)) {};

        [[nodiscard]] const int &get_freq_index() const;

        [[nodiscard]] const int64_t & get_window() const;

        [[nodiscard]] const float &get_time() const;

        /**
         * Check whether the current peak and the other belong to
         * the same band
         * @param peak Other peak to compare
         * @return true if in the same band, false otherwise
         */
        [[nodiscard]] bool same_band(const peak &peak) const;

        /**
         * Operator to decreasingly sort peaks by power
         * @param peak Other peak
         * @return true if this.power < other.power, false otherwise
         */
        bool operator<(const peak &peak) const;

        /**
         * Operator to decreasingly sort peaks by power
         * @param peak Other peak
         * @return true if this.power > other.power, false otherwise
         */
        bool operator>(const peak &peak) const;

    };
}

#endif
