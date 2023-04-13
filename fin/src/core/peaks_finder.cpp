#include <fin/core/peaks_finder.h>
#include <fin/utils/fixed_size_pq.h>
#include <fin/math/fft_window.h>
#include <algorithm>

static fin::utils::FixedSizePQ<fin::core::Peak, consts::fingerprint::N_PEAKS>
findPeaksInWindow(const fin::math::FFTWindow &fftWindow, const std::int64_t &window, const int &bandStart,
                  const int &bandEnd) {
    fin::utils::FixedSizePQ <fin::core::Peak, consts::fingerprint::N_PEAKS> peaks;
    float magCurrent, freqCurrent;
    int indexLeft, indexRight, j;
    bool ok = false;

    for (int i = bandStart; i <= bandEnd; i++) {
        freqCurrent = fin::math::window::FREQ_BINS[i];

        //The peak freq must be between the two boundaries
        if ((freqCurrent < consts::fingerprint::MIN_FREQ) || (freqCurrent > consts::fingerprint::MAX_FREQ))
            continue; //Maybe ugly, but early exit helps a lot

        //Get the current peak value
        magCurrent = fftWindow.getMagnitudes()[i];

        /*
         * Extract respectively 5 element after and before 'i'.
         * Are there 5 element before and after actually?
         */
        indexLeft = i - consts::fingerprint::PEAK_RANGE >= bandStart ? i - consts::fingerprint::PEAK_RANGE : bandStart;
        indexRight =
                i + consts::fingerprint::PEAK_RANGE + 1 <= bandEnd ? i + consts::fingerprint::PEAK_RANGE + 1 : bandEnd;

        //while we don't reach the current peak and the analyzed peak is less loud than the current one
        for (; indexLeft != i && (ok = fftWindow.getMagnitudes()[indexLeft] < magCurrent); indexLeft++);
        if (!ok)
            continue;

        //same as above
        for (j = i + 1; j <= indexRight && (ok = fftWindow.getMagnitudes()[j] < magCurrent); j++);
        if (!ok) {
            //if @ j there's a loudest peak, we can skip all the peaks between 'i' and 'j-1'
            i = j - 1; //for will increment it
            continue;
        }

        peaks.insert(fin::core::Peak(i, magCurrent, window));
    }

    return peaks;
}

std::vector<fin::core::Peak> fin::core::findPeaks(const math::Spectrogram &spectrogram) {
    int currBand, nextBand;
    std::vector<Peak> peakVec; //vector to be returned
    utils::FixedSizePQ<Peak, consts::fingerprint::N_PEAKS> tmp; //to store the temporary loudest peaks

    //For each band
    for (std::int64_t b = 0; b < static_cast<std::int64_t>(math::window::BANDS.size()) - 1; b++) {
        currBand = math::window::BANDS[b];
        nextBand = math::window::BANDS[b + 1];

        //For each window in the spectrogram
        for (std::int64_t i = 0; i < static_cast<std::int64_t>(spectrogram.size()); i++) {

            //Every C, or at the end of the window, add tmp to peakVec then reset tmp
            if (i % consts::fingerprint::C == 0 || i == static_cast<std::int64_t>(spectrogram.size()) - 1) {
                peakVec.insert(peakVec.end(), tmp.begin(), tmp.end());
                tmp.clear();
            }

            //Actually find the peaks between the two BANDS
            auto found_peaks = findPeaksInWindow(spectrogram[i], i, currBand, nextBand - 1);
            for (const auto &peak: found_peaks) //Copy the found peaks in the tmp peaks holder
                tmp.insert(peak);
        }
    }

    std::sort(peakVec.begin(), peakVec.end()); //Sort in descending order by loudness
    return peakVec;
}
