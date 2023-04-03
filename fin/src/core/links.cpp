#include "../../include/core/links.h"
#include <cmath>
#include <algorithm>

core::links::links(const std::vector<peak> &peak_vec) {
    float time_step = (float) consts::window::step_size / consts::audio::SAMPLE_RATE; //Time step between each window
    //Get the maximum window distance in terms of array index given the float time difference
    std::int64_t max_win_distance = std::lround(consts::Links::MaxWinDistanceF / time_step);

    std::vector<peak>::const_iterator left, right;

    for (auto it = peak_vec.begin(); it != peak_vec.end(); it++) {
        //iterator pointing to the first left peak to be considered, if there's enough room
        left = it - max_win_distance >= peak_vec.begin() ? it - max_win_distance : peak_vec.begin();
        //same as above, but pointing to the right most peak
        right = it + max_win_distance <= peak_vec.end() ? it + max_win_distance : peak_vec.end();

        for (; left <= right; left++)
            if (std::abs(left->get_time() - it->get_time()) >= consts::Links::MinWinDistanceF && left->same_band(*it))
                this->emplace_back(*it, *left);
    }
}
