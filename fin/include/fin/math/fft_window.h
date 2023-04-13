#ifndef FIN_MATH_FFT_WINDOW_H
#define FIN_MATH_FFT_WINDOW_H

#include <array>
#include <vector>
#include <complex>
#include "../../../consts.h"

namespace fin::math {
    /**
     * Simple container class to store the output of the
     * FFT and the corresponding time
     */
    class FFTWindow {
        friend class Spectrogram;

    private:
        std::array<float, consts::window::FREQ_BINS> magnitudes_{};

    public:
        [[nodiscard]] const std::array<float, consts::window::FREQ_BINS> &getMagnitudes() const {
            return magnitudes_;
        }
    };
}


#endif
