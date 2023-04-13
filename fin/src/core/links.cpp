#include <fin/core/links.h>
#include <cmath>
#include <algorithm>

fin::core::Links::Links(const std::vector<Peak> &peakVec) {
    for (auto peakIt = peakVec.begin(); peakIt != peakVec.end(); peakIt++) {
        for (auto it = peakIt + 1; it != peakVec.end(); it++) {
            //peakIt is the current peak under analysis, the anchor point candidate
            //it is a candidate peak which could be expressed wrt to peakIt

            if ((it->sameBand(*peakIt)) &&
                (it->getWindow() - peakIt->getWindow() >= consts::links::MIN_WIN_DISTANCE) &&
                (it->getWindow() - peakIt->getWindow() < consts::links::MAX_WIN_DISTANCE)) {

                emplace_back(*peakIt, *it);
            }
        }
    }
}

fin::utils::ByteBuffer fin::core::Links::toByteBuffer() {
    fin::utils::ByteBuffer byteBuffer;

    byteBuffer.add(static_cast<std::uint64_t>(size()));
    for (auto const &link: *this) {
        byteBuffer.add(link.getTime());
        byteBuffer.add(link.getHash());
    }

    return byteBuffer;
}

fin::core::Links fin::core::Links::fromByteBuffer(utils::ByteBuffer &byteBuffer) {
    Links links;

    std::uint64_t len;
    byteBuffer.remove(len);
    std::uint64_t time, hash;

    for (decltype(len) i = 0; i < len; i++) {
        byteBuffer.remove(time);
        byteBuffer.remove(hash);

        links.emplace_back(hash, time);
    }

    return links;
}
