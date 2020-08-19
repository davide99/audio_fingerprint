#ifndef UTILS_WRAPPER_H
#define UTILS_WRAPPER_H

#include <string>
#include "../IO/DB.h"
#include "../IO/WavReader.h"
#include "../Math/Spectrogram.h"
#include "../Core/Fingerprint.h"

/*
 * Just a wrapper around the two main functions
 */
namespace Utils {
    /**
     * Insert a song in the database
     * @param fileName of the song
     * @param db initialized db object
     */
    void insertSong(const std::string &fileName, IO::DB &db) {
        IO::WavReader wavReader(fileName);
        Math::Spectrogram spectrogram(wavReader.getData());
        std::vector<Core::Peak> peaks = Core::Fingerprint::compute(spectrogram);
        Core::Links links = Core::Links(peaks);

        db.insertSong(fileName, links);
    }

    /**
     * Try to find a match for a recording
     * @param fileName of the recording
     * @param db initialized db object
     * @return the name of the sonf if found, an empty string otherwise
     */
    std::string search(const std::string &fileName, IO::DB &db) {
        IO::WavReader wavMic(fileName);
        Math::Spectrogram specMic(wavMic.getData());
        std::vector<Core::Peak> peaksMic = Core::Fingerprint::compute(specMic);
        Core::Links linksMic = Core::Links(peaksMic);

        std::uint64_t id;
        if (db.searchIdGivenLinks(linksMic, id))
            return db.getSongNameById(id);
        else
            return "";
    }
};


#endif
