#ifndef FIN_MATH_SPECTROGRAM_H
#define FIN_MATH_SPECTROGRAM_H

#include <vector>
#include <fin/math/fft_window.h>
#include "../../../consts.h"

namespace fin::math {
    /**
     * Class to compute the spectrogram given the samples
     */
    class spectrogram {

    public:
        /**
         * Constructs the spectrogram
         * @param data samples
         */
        explicit spectrogram(const std::vector<float>& data);

    private:
        std::vector<fft_window> fft_windows;
    public:
        /**
         * Get a specific FFT window
         * @param pos what window
         * @return the window
         */
        const fft_window &operator[](std::size_t pos) const;

        /**
         * @return the number of FFT windows
         */
        [[nodiscard]] std::size_t size() const;
    };
}

#endif
