#include <fin/math/window.h>
#include <cmath>

static std::array<float, consts::window::size> make_window() noexcept {
    std::array<float, consts::window::size> tmp{};

    for (int i = 0; i < consts::window::size; i++)
        tmp[i] = (float) (0.5f * (1 - std::cos(2 * M_PI * i / (consts::window::size - 1)))); //Hanning

    return tmp;
}

const std::array<float, consts::window::size> fin::math::window::window = make_window();

//----------------------------------------------------------------------

static std::array<float, consts::window::freq_bins> make_freq_bins() noexcept {
    std::array<float, consts::window::freq_bins> tmp{};

    for (int i = 0; i < consts::window::freq_bins; i++)
        tmp[i] = (consts::audio::SAMPLE_RATE / 2.0f) * ((float) i / consts::window::freq_bins);


    return tmp;
}

const std::array<float, consts::window::freq_bins> fin::math::window::freq_bins = make_freq_bins();

//----------------------------------------------------------------------

static std::vector<int> make_bands() noexcept {
    std::vector<int> tmp{};

    int freqIndex;
    float factor = 700.0f / consts::window::FreqBinStep;

    for (auto mel = consts::window::MelStart;; mel += consts::window::MelStep) {
        freqIndex = (int) std::round(factor * (std::pow(10.0f, (float) mel / 2595.0f) - 1.0f));

        if (freqIndex > consts::window::freq_bins)
            break;
        else
            tmp.push_back(freqIndex);
    }

    return tmp;
}

const std::vector<int> fin::math::window::bands = make_bands();

//----------------------------------------------------------------------

static std::array<int, consts::window::freq_bins> make_bands_map() noexcept {
    std::array<int, consts::window::freq_bins> tmp{};

    auto it = fin::math::window::bands.begin();
    int bandIndex = 0;

    for (int i = 0; i < consts::window::freq_bins; i++) {
        if (i >= *it && it != fin::math::window::bands.end()) { //next band?
            it++;
            bandIndex++;
        }

        tmp[i] = bandIndex;
    }

    return tmp;
}

const std::array<int, consts::window::freq_bins> fin::math::window::bands_map = make_bands_map();

const int &fin::math::window::get_band_index(const int &freq_index) {
    return bands_map[freq_index];
}
