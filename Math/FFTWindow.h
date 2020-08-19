#ifndef STRUCTURES_FFTWINDOW_H
#define STRUCTURES_FFTWINDOW_H

#include <array>
#include <vector>
#include <complex>
#include "../Consts.h"

namespace Math {
    /**
     * Simple container class to store the output of the
     * FFT and the corresponding time
     */
    class FFTWindow {
        friend class Spectrogram;

    private:
        std::array<float, Consts::Window::FreqBins> magnitudes;
        float time{};

    public:
        [[nodiscard]] const std::array<float, Consts::Window::FreqBins> &getMagnitudes() const {
            return magnitudes;
        }

        [[nodiscard]] const float &getTime() const {
            return time;
        }
    };
}


#endif
