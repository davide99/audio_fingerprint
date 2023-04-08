#include <fin/math/window.h>
#include <cmath>

static std::array<float, consts::window::SIZE> make_window() noexcept {
    std::array<float, consts::window::SIZE> tmp{};

    for (int i = 0; i < consts::window::SIZE; i++)
        tmp[i] = (float) (0.5f * (1 - std::cos(2 * M_PI * i / (consts::window::SIZE - 1)))); //Hanning

    return tmp;
}

const std::array<float, consts::window::SIZE> fin::math::window::WINDOW = make_window();

//----------------------------------------------------------------------

static std::array<float, consts::window::FREQ_BINS> make_freq_bins() noexcept {
    std::array<float, consts::window::FREQ_BINS> tmp{};

    for (int i = 0; i < consts::window::FREQ_BINS; i++)
        tmp[i] = (consts::audio::SAMPLE_RATE / 2.0f) * ((float) i / consts::window::FREQ_BINS);


    return tmp;
}

const std::array<float, consts::window::FREQ_BINS> fin::math::window::FREQ_BINS = make_freq_bins();

//----------------------------------------------------------------------

static std::vector<int> make_bands() noexcept {
    std::vector<int> tmp{};

    int freqIndex;
    float factor = 700.0f / consts::window::FREQ_BIN_STEP;

    for (auto mel = consts::window::MEL_START;; mel += consts::window::MEL_STEP) {
        freqIndex = (int) std::round(factor * (std::pow(10.0f, (float) mel / 2595.0f) - 1.0f));

        if (freqIndex > consts::window::FREQ_BINS)
            break;
        else
            tmp.push_back(freqIndex);
    }

    return tmp;
}

const std::vector<int> fin::math::window::BANDS = make_bands();

//----------------------------------------------------------------------

static std::array<int, consts::window::FREQ_BINS> make_bands_map() noexcept {
    std::array<int, consts::window::FREQ_BINS> tmp{};

    auto it = fin::math::window::BANDS.begin();
    int bandIndex = 0;

    for (int i = 0; i < consts::window::FREQ_BINS; i++) {
        if (i >= *it && it != fin::math::window::BANDS.end()) { //next band?
            it++;
            bandIndex++;
        }

        tmp[i] = bandIndex;
    }

    return tmp;
}

const std::array<int, consts::window::FREQ_BINS> fin::math::window::BANDS_MAP = make_bands_map();

const int &fin::math::window::getBandIndex(const int &freqIndex) {
    return BANDS_MAP[freqIndex];
}
