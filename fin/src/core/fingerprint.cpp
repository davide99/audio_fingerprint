#include <fin/core/fingerprint.h>
#include <fin/core/peaks_finder.h>
#include <algorithm>

std::vector<fin::core::peak> fin::core::fingerprint::compute(const math::spectrogram &spectrogram) {
    int currBand, nextBand;
    std::vector<peak> peak_vec; //vector to be returned
    utils::fixed_size_pq<peak, consts::fingerprint::n_peaks> tmp; //to store the temporary loudest peaks

    //For each band
    for (std::int64_t b = 0; b < static_cast<std::int64_t>(math::window::bands.size()) - 1; b++) {
        currBand = math::window::bands[b];
        nextBand = math::window::bands[b + 1];

        //For each window in the spectrogram
        for (std::int64_t i = 0; i < static_cast<std::int64_t>(spectrogram.size()); i++) {

            //Every C, or at the end of the window, add tmp to peak_vec then reset tmp
            if (i % consts::fingerprint::C == 0 || i == static_cast<std::int64_t>(spectrogram.size()) - 1) {
                peak_vec.insert(peak_vec.end(), tmp.begin(), tmp.end());
                tmp.clear();
            }

            //Actually find the peaks between the two bands
            auto found_peaks = find_peaks(spectrogram[i], i, currBand, nextBand - 1);
            for (const auto &peak : found_peaks) //Copy the found peaks in the tmp peaks holder
                tmp.insert(peak);
        }
    }

    std::sort(peak_vec.begin(), peak_vec.end()); //Sort in descending order by loudness
    return peak_vec;
}
