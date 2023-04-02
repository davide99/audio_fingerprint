#include <fftw3.h>
#include "Spectrogram.h"
#include "Vector.h"
#include "Window.h"

//The output length of an FFT for an x real-valued input array is x.length / 2 + 1
static constexpr std::uint16_t FFTOutSize = Consts::Window::Size / 2 + 1;

Math::Spectrogram::Spectrogram(const std::vector<float> &data) {
    //Calculation of the winFFT size with integer roundup
    std::size_t winFFTsize = ((std::size_t) ((data.size() - Consts::Window::Size) / Consts::Window::StepSize)) *
                             Consts::Window::StepSize;
    this->fftWindows.reserve(winFFTsize);

    FFTWindow fftWindow;
    float timeWindow[Consts::Window::Size]; //fft input
    fftwf_complex fftOut[FFTOutSize];  //fft output

    fftwf_plan p = fftwf_plan_dft_r2c_1d(Consts::Window::Size, timeWindow, fftOut, FFTW_ESTIMATE);

    for (std::size_t i = 0; i + Consts::Window::Size < data.size(); i += Consts::Window::StepSize) {
        //Multiply the sliding window by the hamming window
        Math::Vector::mul(Window::window.data(), data.data() + i, timeWindow, Consts::Window::Size);
        fftwf_execute(p);

        /*
         * The first bin in the FFT output is the DC component, get rid of it by starting at fftOut+1
         * and save just the magnitude, not the complex number
         */
        std::transform(fftOut + 1, fftOut + FFTOutSize, fftWindow.magnitudes.data(),
                       [](const fftwf_complex &i) -> float {
                           return std::sqrt(i[0] * i[0] + i[1] * i[1]);
                       });

        fftWindow.time = (float) i / Consts::Audio::SampleRate;
        this->fftWindows.push_back(fftWindow);
    }

    fftwf_destroy_plan(p);
}

const Math::FFTWindow &Math::Spectrogram::operator[](std::size_t pos) const {
    return this->fftWindows[pos];
}

std::size_t Math::Spectrogram::size() const {
    return this->fftWindows.size();
}
