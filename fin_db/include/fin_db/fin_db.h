#ifndef FIN_DB_WRAPPER_H
#define FIN_DB_WRAPPER_H

#include <string>
#include <fin/math/spectrogram.h>
#include <fin/core/links.h>
#include <fin/readers/reader.h>
#include <fin/readers/wav_reader.h>
#include <fin/fin.h>
#include <fin_db/db.h>

/*
 * Just a wrapper around the two main functions
 */
namespace fin {
    /**
    * Try to find a match given some links
    * @param links of the recording
    * @param db initialized db object
    * @return the name of the song if found, an empty string otherwise
    */
    fin::DB::SearchResult searchFromLinks(const fin::core::Links &links, DB &db) {
        auto result = db.searchSongGivenLinks(links);

        if (result.found)
            result.name = db.getSongNameById(result.id);

        return result;
    }

    /**
    * Insert a song in the database
    * @param filename of the song
    * @param db initialized db object
    */
    void insertSong(const std::string &filename, DB &db) {
        fin::readers::WavReader wavReader(filename);
        fin::core::Links links = fin::computeLinks(wavReader);

        db.insertSong(wavReader.getBasename(), links);
    }
}

#endif