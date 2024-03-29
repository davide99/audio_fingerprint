#include <fin/core/links.h>
#include <cmath>
#include <algorithm>

fin::core::Links::Links(std::vector<Peak> &peakVec) {
    //Sort in ascending order by window
    std::sort(peakVec.begin(), peakVec.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.getWindow() < rhs.getWindow();
    });

    for (auto anchorIt = peakVec.begin(); anchorIt != peakVec.end(); anchorIt++) {
        for (auto it = anchorIt + 1;
             (it->getWindow() - anchorIt->getWindow() < consts::links::MAX_WIN_DISTANCE) &&
             (it != peakVec.end());
             it++) {
            if (it->sameBand(*anchorIt) &&
                (it->getWindow() - anchorIt->getWindow() >= consts::links::MIN_WIN_DISTANCE)) {
                emplace_back(*anchorIt, *it);
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
