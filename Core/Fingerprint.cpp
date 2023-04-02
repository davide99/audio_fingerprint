#include "Fingerprint.h"
#include "../Utils/FixedSizePQ.h"
#include "PeaksFinder.h"
#include <algorithm>

std::vector<Core::Peak> Core::Fingerprint::compute(const Math::Spectrogram &spectrogram) {
    int currBand, nextBand;
    std::vector<Peak> peakVec; //vector to be returned
    Utils::FixedSizePQ<Peak, Consts::Fingerprint::NPeaks> tmp; //to store the temporary loudest peaks

    //For each band
    for (std::int64_t b = 0; b < Math::Window::bands.size() - 1; b++) {
        currBand = Math::Window::bands[b];
        nextBand = Math::Window::bands[b + 1];

        //For each window in the spectrogram
        for (std::int64_t i = 0; i < spectrogram.size(); i++) {

            //Every C, or at the end of the window, add tmp to peakVec then reset tmp
            if (i % Consts::Fingerprint::C == 0 || i == spectrogram.size() - 1) {
                peakVec.insert(peakVec.end(), tmp.begin(), tmp.end());
                tmp.clear();
            }

            //Actually find the peaks between the two bands
            auto foundPeaks = findPeaks(spectrogram[i], i, currBand, nextBand - 1);
            for (const auto &peak : foundPeaks) //Copy the found peaks in the tmp peaks holder
                tmp.insert(peak);
        }
    }

    std::sort(peakVec.begin(), peakVec.end()); //Sort in descending order by loudness
    return peakVec;
}
