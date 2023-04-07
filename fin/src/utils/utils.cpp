#include <fin/utils/utils.h>
#include <cstdint>

bool fin::utils::is_big_endian() {
    uint16_t n = 0x0001u;
    auto *ptr = reinterpret_cast<uint8_t *>(&n);

    return ptr[0] == 0;
}

fin::core::links fin::utils::computeLinks(fin::readers::reader &reader) {
    fin::math::spectrogram spectrogram(reader.get_data());
    std::vector<fin::core::peak> peaks = fin::core::fingerprint::compute(spectrogram);
    return fin::core::links(peaks);
}
