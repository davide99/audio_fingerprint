#include <fin/fin.h>
#include <fin/math/spectrogram.h>
#include <fin/core/fingerprint.h>

fin::core::Links fin::computeLinks(fin::readers::Reader &reader) {
    fin::math::Spectrogram spectrogram(reader.getData());
    std::vector<fin::core::Peak> peaks = fin::core::fingerprint::compute(spectrogram);
    return fin::core::Links(peaks);
}
