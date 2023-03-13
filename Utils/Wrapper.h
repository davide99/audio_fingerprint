#ifndef UTILS_WRAPPER_H
#define UTILS_WRAPPER_H

#include <string>
#ifdef BUILD_SERVER
#include "../IO/DB.h"
#include "../IO/Readers/WavReader.h"
#endif
#include "../Math/Spectrogram.h"
#include "../Core/Fingerprint.h"
#include "../Core/Links.h"
#include "../IO/Readers/Reader.h"

/*
 * Just a wrapper around the two main functions
 */
namespace Utils {
    /**
    * Compute links given a generic reader
    * @param reader where to read samples from
    * @return computed links
    */
    Core::Links computeLinks(IO::Readers::Reader &reader) {
        Math::Spectrogram spectrogram(reader.getData());
        std::vector<Core::Peak> peaks = Core::Fingerprint::compute(spectrogram);
        return Core::Links(peaks);
    }

#ifdef BUILD_SERVER
    /**
    * Try to find a match given some links
    * @param links of the recording
    * @param db initialized db object
    * @return the name of the song if found, an empty string otherwise
    */
    std::string searchFromLinks(const Core::Links &links, IO::DB &db) {
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
    void insertSong(const std::string &fileName, IO::DB &db) {
        IO::Readers::WavReader wavReader(fileName);
        Core::Links links = computeLinks(wavReader);

        db.insertSong(fileName, links);
    }
#endif
}

#endif
