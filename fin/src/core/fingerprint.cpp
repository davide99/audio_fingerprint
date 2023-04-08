#include <fin/core/fingerprint.h>
#include <fin/core/peaks_finder.h>
#include <algorithm>

std::vector<fin::core::Peak> fin::core::fingerprint::compute(const math::Spectrogram &spectrogram) {
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
            auto found_peaks = findPeaks(spectrogram[i], i, currBand, nextBand - 1);
            for (const auto &peak : found_peaks) //Copy the found peaks in the tmp peaks holder
                tmp.insert(peak);
        }
    }

    std::sort(peakVec.begin(), peakVec.end()); //Sort in descending order by loudness
    return peakVec;
}
