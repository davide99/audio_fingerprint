#include <fin/utils/utils.h>
#include <fin/math/spectrogram.h>
#include <fin/core/fingerprint.h>
#include <cstdint>

bool fin::utils::isBigEndian() {
    uint16_t n = 0x0001u;
    auto *ptr = reinterpret_cast<uint8_t *>(&n);

    return ptr[0] == 0;
}

fin::core::Links fin::utils::computeLinks(fin::readers::Reader &reader) {
    fin::math::Spectrogram spectrogram(reader.getData());
    std::vector<fin::core::Peak> peaks = fin::core::fingerprint::compute(spectrogram);
    return fin::core::Links(peaks);
}
