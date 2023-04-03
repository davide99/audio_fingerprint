#ifndef UTILS_WRAPPER_H
#define UTILS_WRAPPER_H

#include <string>
#include <fin/math/spectrogram.h>
#include <fin/core/fingerprint.h>
#include <fin/core/links.h>
#include <fin/readers/reader.h>
#include "db.h"
#include "wav_reader.h"

/*
 * Just a wrapper around the two main functions
 */
namespace utils {
    /**
    * Compute links given a generic reader
    * @param reader where to read samples from
    * @return computed links
    */
    fin::core::links computeLinks(fin::readers::reader &reader) {
        fin::math::spectrogram spectrogram(reader.get_data());
        std::vector<fin::core::peak> peaks = fin::core::fingerprint::compute(spectrogram);
        return fin::core::links(peaks);
    }

    /**
    * Try to find a match given some links
    * @param links of the recording
    * @param db initialized db object
    * @return the name of the song if found, an empty string otherwise
    */
    std::string searchFromLinks(const fin::core::links &links, db &db) {
        std::uint64_t id;
        if (db.searchIdGivenLinks(links, id))
            return db.getSongNameById(id);
        else
            return "";
    }

    /**
    * Insert a song in the database
    * @param fileName of the song
    * @param db initialized db object
    */
    void insertSong(const std::string &fileName, db &db) {
        wav_reader wavReader(fileName);
        fin::core::links links = computeLinks(wavReader);

        db.insertSong(fileName, links);
    }
}

#endif