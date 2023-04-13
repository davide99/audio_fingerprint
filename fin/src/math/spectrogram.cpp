#include <fftw3.h>
#include <fin/math/spectrogram.h>
#include <fin/math/vector.h>
#include <fin/math/window.h>

//The output length of an FFT for an x real-valued input array is x.length / 2 + 1
static constexpr std::uint16_t fft_out_size = consts::window::SIZE / 2 + 1;

fin::math::Spectrogram::Spectrogram(const std::vector<float> &data) {
    //Calculation of the winFFT size with integer roundup
    std::size_t winFFTSize = ((std::size_t) ((data.size() - consts::window::SIZE) / consts::window::STEP_SIZE)) *
                             consts::window::STEP_SIZE;
    fftWindows_.reserve(winFFTSize);

    FFTWindow fftWindow;
    float timeWindow[consts::window::SIZE]; //fft input
    fftwf_complex fftOut[fft_out_size];  //fft output

    fftwf_plan p = fftwf_plan_dft_r2c_1d(consts::window::SIZE, timeWindow, fftOut, FFTW_ESTIMATE);

    for (std::size_t i = 0; i + consts::window::SIZE < data.size(); i += consts::window::STEP_SIZE) {
        //Multiply the sliding window by the hamming window
        math::vector::mul(window::WINDOW.data(), data.data() + i, timeWindow, consts::window::SIZE);
        fftwf_execute(p);

        /*
         * The first bin in the FFT output is the DC component, get rid of it by starting at fftOut+1
         * and save just the magnitude, not the complex number
         */
        std::transform(fftOut + 1, fftOut + fft_out_size, fftWindow.magnitudes_.data(),
                       [](const fftwf_complex &i) -> float {
                           return 10 * std::log10(i[0] * i[0] + i[1] * i[1]);
                       });

        fftWindows_.push_back(fftWindow);
    }

    fftwf_destroy_plan(p);
}

const fin::math::FFTWindow &fin::math::Spectrogram::operator[](std::size_t pos) const {
    return fftWindows_[pos];
}

std::size_t fin::math::Spectrogram::size() const {
    return fftWindows_.size();
}
