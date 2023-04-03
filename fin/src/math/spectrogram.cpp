#include <fftw3.h>
#include "../../include/math/spectrogram.h"
#include "../../include/math/vector.h"
#include "../../include/math/window.h"

//The output length of an FFT for an x real-valued input array is x.length / 2 + 1
static constexpr std::uint16_t fft_out_size = consts::window::size / 2 + 1;

math::spectrogram::spectrogram(const std::vector<float> &data) {
    //Calculation of the winFFT size with integer roundup
    std::size_t win_fft_size = ((std::size_t) ((data.size() - consts::window::size) / consts::window::step_size)) *
                               consts::window::step_size;
    this->fft_windows.reserve(win_fft_size);

    fft_window fft_window;
    float timeWindow[consts::window::size]; //fft input
    fftwf_complex fftOut[fft_out_size];  //fft output

    fftwf_plan p = fftwf_plan_dft_r2c_1d(consts::window::size, timeWindow, fftOut, FFTW_ESTIMATE);

    for (std::size_t i = 0; i + consts::window::size < data.size(); i += consts::window::step_size) {
        //Multiply the sliding window by the hamming window
        math::vector::mul(window::window.data(), data.data() + i, timeWindow, consts::window::size);
        fftwf_execute(p);

        /*
         * The first bin in the FFT output is the DC component, get rid of it by starting at fftOut+1
         * and save just the magnitude, not the complex number
         */
        std::transform(fftOut + 1, fftOut + fft_out_size, fft_window.magnitudes.data(),
                       [](const fftwf_complex &i) -> float {
                           return std::sqrt(i[0] * i[0] + i[1] * i[1]);
                       });

        fft_window.time = (float) i / consts::audio::SAMPLE_RATE;
        this->fft_windows.push_back(fft_window);
    }

    fftwf_destroy_plan(p);
}

const math::fft_window &math::spectrogram::operator[](std::size_t pos) const {
    return this->fft_windows[pos];
}

std::size_t math::spectrogram::size() const {
    return this->fft_windows.size();
}
