#ifndef STRUCTURES_FFTWINDOW_H
#define STRUCTURES_FFTWINDOW_H

#include <array>
#include <vector>
#include <complex>
#include "../../../consts.h"

namespace fin::math {
    /**
     * Simple container class to store the output of the
     * FFT and the corresponding time
     */
    class fft_window {
        friend class spectrogram;

    private:
        std::array<float, consts::window::freq_bins> magnitudes{};
        float time{};

    public:
        [[nodiscard]] const std::array<float, consts::window::freq_bins> &get_magnitudes() const {
            return magnitudes;
        }

        [[nodiscard]] const float &getTime() const {
            return time;
        }
    };
}


#endif
