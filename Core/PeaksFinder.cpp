#include "PeaksFinder.h"

Utils::FixedSizePQ<Core::Peak, Consts::Fingerprint::NPeaks>
Core::findPeaks(const Math::FFTWindow &fftWindow, const size_t &window, const int &bandStart, const int &bandEnd) {
    Utils::FixedSizePQ<Peak, Consts::Fingerprint::NPeaks> peaks;
    float magCurrent, freqCurrent;
    int indexLeft, indexRight, j;
    bool ok;

    for (int i = bandStart; i <= bandEnd; i++) {
        freqCurrent = Math::Window::freqBins[i];

        //The peak freq must be between the two boundaries
        if ((freqCurrent < Consts::Fingerprint::MinFreq) || (freqCurrent > Consts::Fingerprint::MaxFreq))
            continue; //Maybe ugly, but early exit helps a lot

        //Get the current peak value
        magCurrent = fftWindow.getMagnitudes()[i];

        /*
         * Extract respectively 5 element after and before i.
         * Are there 5 element before and after actually?
         */
        indexLeft = i - Consts::Fingerprint::PeakRange >= bandStart ? i - Consts::Fingerprint::PeakRange : bandStart;
        indexRight =
                i + Consts::Fingerprint::PeakRange + 1 <= bandEnd ? i + Consts::Fingerprint::PeakRange + 1 : bandEnd;

        //while we don't reach the current peak and the analyzed peak is less loud than the current one
        for (; indexLeft != i && (ok = fftWindow.getMagnitudes()[indexLeft] < magCurrent); indexLeft++);
        if (!ok)
            continue;

        //same as above
        for (j = i + 1; j <= indexRight && (ok = fftWindow.getMagnitudes()[j] < magCurrent); j++);
        if (!ok) {
            //if @ j there's a loudest peak, we can skip all the peaks between i and j-1
            i = j - 1; //for will increment it
            continue;
        }

        peaks.insert(Peak(i, magCurrent, window, fftWindow.getTime()));
    }

    return peaks;
}
