#include <fin/core/link.h>
#include <fin/utils/utils.h>
#include <fin/math/integers.h>
#include <xxh3.h>

fin::core::Link::Link(const Peak &address, const Peak &peak) {
    std::int64_t deltaTime = peak.getWindow() - address.getWindow();
    std::int64_t deltaFreq = peak.getFreqIndex() - address.getFreqIndex();
    std::uint64_t addrFreq = address.getFreqIndex();

    XXH3_state_t state;
    XXH3_64bits_reset(&state);

    if (utils::isBigEndian()) {
        deltaTime = math::integers::byteSwap(deltaTime);
        deltaFreq = math::integers::byteSwap(deltaFreq);
        addrFreq = math::integers::byteSwap(addrFreq);
    }

    XXH3_64bits_update(&state, &deltaTime, sizeof(deltaTime));
    XXH3_64bits_update(&state, &deltaFreq, sizeof(deltaFreq));
    XXH3_64bits_update(&state, &addrFreq, sizeof(addrFreq));

    hash_ = XXH3_64bits_digest(&state);
    window_ = address.getWindow();
}

fin::core::Link::Link(const uint64_t &hash, const uint64_t &window) {
    hash_ = hash;
    window_ = window;
}

const std::uint64_t &fin::core::Link::getHash() const {
    return hash_;
}

const std::uint64_t &fin::core::Link::getTime() const {
    return window_;
}
