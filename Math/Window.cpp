#include "Window.h"
#include <cmath>

static std::array<float, Consts::Window::Size> makeWindow() noexcept {
    std::array<float, Consts::Window::Size> tmp{};

    for (int i = 0; i < Consts::Window::Size; i++)
        tmp[i] = (float) (0.5f * (1 - std::cos(2 * M_PI * i / (Consts::Window::Size - 1)))); //Hanning

    return tmp;
}

const std::array<float, Consts::Window::Size> Math::Window::window = makeWindow();

//----------------------------------------------------------------------

static std::array<float, Consts::Window::FreqBins> makeFreqBins() noexcept {
    std::array<float, Consts::Window::FreqBins> tmp{};

    for (int i = 0; i < Consts::Window::FreqBins; i++)
        tmp[i] = (Consts::Audio::SampleRate / 2.0f) * ((float) i / Consts::Window::FreqBins);


    return tmp;
}

const std::array<float, Consts::Window::FreqBins> Math::Window::freqBins = makeFreqBins();

//----------------------------------------------------------------------

static std::vector<int> makeBands() noexcept {
    std::vector<int> tmp{};

    int freqIndex;
    float factor = 700.0f / Consts::Window::FreqBinStep;

    for (auto mel = Consts::Window::MelStart;; mel += Consts::Window::MelStep) {
        freqIndex = (int) std::round(factor * (std::pow(10.0f, (float) mel / 2595.0f) - 1.0f));

        if (freqIndex > Consts::Window::FreqBins)
            break;
        else
            tmp.push_back(freqIndex);
    }

    return tmp;
}

const std::vector<int> Math::Window::bands = makeBands();

//----------------------------------------------------------------------

static std::array<int, Consts::Window::FreqBins> makeBandsMap() noexcept {
    std::array<int, Consts::Window::FreqBins> tmp{};

    auto it = Math::Window::bands.begin();
    int bandIndex = 0;

    for (int i = 0; i < Consts::Window::FreqBins; i++) {
        if (i >= *it && it != Math::Window::bands.end()) { //next band?
            it++;
            bandIndex++;
        }

        tmp[i] = bandIndex;
    }

    return tmp;
}

const std::array<int, Consts::Window::FreqBins> Math::Window::bandsMap = makeBandsMap();

const int &Math::Window::getBandIndex(const int &freqIndex) {
    return bandsMap[freqIndex];
}
