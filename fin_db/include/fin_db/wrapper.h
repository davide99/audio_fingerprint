#ifndef FIN_DB_WRAPPER_H
#define FIN_DB_WRAPPER_H

#include <string>
#include <fin/math/spectrogram.h>
#include <fin/core/fingerprint.h>
#include <fin/core/links.h>
#include <fin/readers/reader.h>
#include <fin/readers/wav_reader.h>
#include <fin/utils/utils.h>
#include <fin_db/db.h>

/*
 * Just a wrapper around the two main functions
 */
namespace fin::utils {
    /**
    * Try to find a match given some links
    * @param links of the recording
    * @param db initialized db object
    * @return the name of the song if found, an empty string otherwise
    */
    std::string searchFromLinks(const fin::core::Links &links, DB &db) {
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
    void insertSong(const std::string &fileName, DB &db) {
        fin::readers::WavReader wavReader(fileName);
        fin::core::Links links = fin::utils::computeLinks(wavReader);

        db.insertSong(fileName, links);
    }
}

#endif