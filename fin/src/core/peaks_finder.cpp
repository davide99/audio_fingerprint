#include "../../include/core/peaks_finder.h"

utils::fixed_size_pq<core::peak, consts::fingerprint::n_peaks>
core::find_peaks(const math::fft_window &fft_window, const int64_t &window, const int &band_start, const int &band_end) {
    utils::fixed_size_pq<peak, consts::fingerprint::n_peaks> peaks;
    float mag_current, freq_current;
    int index_left, index_right, j;
    bool ok;

    for (int i = band_start; i <= band_end; i++) {
        freq_current = math::window::freq_bins[i];

        //The peak freq must be between the two boundaries
        if ((freq_current < consts::fingerprint::MinFreq) || (freq_current > consts::fingerprint::MaxFreq))
            continue; //Maybe ugly, but early exit helps a lot

        //Get the current peak value
        mag_current = fft_window.get_magnitudes()[i];

        /*
         * Extract respectively 5 element after and before 'i'.
         * Are there 5 element before and after actually?
         */
        index_left = i - consts::fingerprint::PeakRange >= band_start ? i - consts::fingerprint::PeakRange : band_start;
        index_right =
                i + consts::fingerprint::PeakRange + 1 <= band_end ? i + consts::fingerprint::PeakRange + 1 : band_end;

        //while we don't reach the current peak and the analyzed peak is less loud than the current one
        for (; index_left != i && (ok = fft_window.get_magnitudes()[index_left] < mag_current); index_left++);
        if (!ok)
            continue;

        //same as above
        for (j = i + 1; j <= index_right && (ok = fft_window.get_magnitudes()[j] < mag_current); j++);
        if (!ok) {
            //if @ j there's a loudest peak, we can skip all the peaks between 'i' and 'j-1'
            i = j - 1; //for will increment it
            continue;
        }

        peaks.insert(peak(i, mag_current, window, fft_window.getTime()));
    }

    return peaks;
}
