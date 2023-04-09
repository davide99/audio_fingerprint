#include <fin/core/links.h>
#include <cmath>
#include <algorithm>

fin::core::Links::Links(const std::vector<Peak> &peakVec) {
    float timeStep = (float) consts::window::STEP_SIZE / consts::audio::SAMPLE_RATE; //Time step between each window
    //Get the maximum window distance in terms of array index given the float time difference
    std::int64_t maxWinDistance = std::lround(consts::links::MAX_WIN_DISTANCE_F / timeStep);

    std::vector<Peak>::const_iterator left, right;

    for (auto it = peakVec.begin(); it != peakVec.end(); it++) {
        //iterator pointing to the first left peak to be considered, if there's enough room
        left = it - maxWinDistance >= peakVec.begin() ? it - maxWinDistance : peakVec.begin();
        //same as above, but pointing to the right most peak
        right = it + maxWinDistance <= peakVec.end() ? it + maxWinDistance : peakVec.end();

        for (; left <= right; left++)
            if (std::abs(left->getTime() - it->getTime()) >= consts::links::MIN_WIN_DISTANCE_F && left->sameBand(*it))
                this->emplace_back(*it, *left);
    }
}

fin::utils::ByteBuffer fin::core::Links::toByteBuffer() {
    fin::utils::ByteBuffer byteBuffer;

    byteBuffer.add64(this->size());
    for (auto const &link: *this) {
        byteBuffer.add64(link.getTime());
        byteBuffer.add64(link.getHash());
    }

    return byteBuffer;
}

fin::core::Links fin::core::Links::fromByteBuffer(utils::ByteBuffer &byteBuffer) {
    Links links;

    auto len = byteBuffer.remove64();
    for (decltype(len) i = 0; i < len; i++) {
        links.emplace_back(byteBuffer.remove64(), byteBuffer.remove64());
    }

    return links;
}
