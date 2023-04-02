#ifndef STRUCTURES_PEAK_H
#define STRUCTURES_PEAK_H

#include "../Consts.h"
#include <cstdint>
#include <type_traits>
#include "../Math/Window.h"

namespace Core {
    /**
     * Class to hold peak information
     */
    class Peak {
    private:
        int freqIndex;       //index referring to an element of Window::freqBins
        float power;         //magnitude
        std::int64_t window; //window the peak belongs to
        float time;          //time in the audio file, related to the window member
        int bandIndex;       //index referring to an element of Window::bands (getBandsIndex)

    public:
        Peak(const int &freqIndex, const float &power, const std::int64_t &window, const float &time) :
                freqIndex(freqIndex), power(power), window(window), time(time),
                bandIndex(Math::Window::getBandIndex(freqIndex)) {};

        [[nodiscard]] const int &getFreqIndex() const;

        [[nodiscard]] const int64_t & getWindow() const;

        [[nodiscard]] const float &getTime() const;

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
