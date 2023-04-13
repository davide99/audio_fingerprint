#include <fin/core/links.h>
#include <cmath>
#include <algorithm>

fin::core::Links::Links(const std::vector<Peak> &peakVec) {
    std::vector<Peak>::const_iterator left, right;

    for (auto it = peakVec.begin(); it != peakVec.end(); it++) {
        //iterator pointing to the first left peak to be considered, if there's enough room
        left = it - consts::links::MAX_WIN_DISTANCE >= peakVec.begin() ?
               it - consts::links::MAX_WIN_DISTANCE : peakVec.begin();
        //same as above, but pointing to the right most peak
        right = it + consts::links::MAX_WIN_DISTANCE <= peakVec.end() ?
                it + consts::links::MAX_WIN_DISTANCE : peakVec.end();

        for (; left <= right; left++)
            if (std::abs(left->getWindow() - it->getWindow()) >= consts::links::MIN_WIN_DISTANCE && left->sameBand(*it))
                emplace_back(*it, *left);
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
