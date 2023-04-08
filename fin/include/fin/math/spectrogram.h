#ifndef FIN_MATH_SPECTROGRAM_H
#define FIN_MATH_SPECTROGRAM_H

#include <vector>
#include <fin/math/fft_window.h>
#include "../../../consts.h"

namespace fin::math {
    /**
     * Class to compute the spectrogram given the samples
     */
    class Spectrogram {

    public:
        /**
         * Constructs the spectrogram
         * @param data samples
         */
        explicit Spectrogram(const std::vector<float>& data);

    private:
        std::vector<FFTWindow> fftWindows;
    public:
        /**
         * Get a specific FFT window
         * @param pos what window
         * @return the window
         */
        const FFTWindow &operator[](std::size_t pos) const;

        /**
         * @return the number of FFT windows
         */
        [[nodiscard]] std::size_t size() const;
    };
}

#endif
