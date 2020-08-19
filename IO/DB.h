#ifndef IO_DB_H
#define IO_DB_H

#include <mariadb++/connection.hpp>
#include <mariadb++/statement.hpp>
#include <cstdint>
#include "../Core/Links.h"

namespace IO {
    /**
     * Class to manage the database
     */
    class DB {
    private:
        mariadb::connection_ref conn;

        mariadb::statement_ref insSongInfoStmt, selSongById;
    public:
        DB();

        ~DB();

        /**
         * @return true if the database exists, false otherwise
         */
        bool exists();

        /**
         * Create the database
         * @return true if no errors occurred, false otherwise
         */
        bool create();

        /**
         * Drop the database
         * @return true if no errors occurred, false otherwise
         */
        bool drop();

        /**
         * Insert a song in the database
         * @param name  Name of the song
         * @param links Song's links
         * @return true if no errors occurred, false otherwise
         */
        bool insertSong(const std::string &name, const Core::Links &links);

        /**
         * Try to find a match for a recording
         * @param links         Recoding's links
         * @param id            Output: found id, untouched if not found
         * @param commonLinks   Output: optional, pointer to common links found
         * @return true if a match is found, false otherwise
         */
        bool searchIdGivenLinks(const Core::Links &links, std::uint64_t &id, std::uint64_t *commonLinks = nullptr);

        /**
         * Find the name of a song
         * @param id Id of the song
         * @return The name if found, an empty string otherwise
         */
        std::string getSongNameById(const std::uint64_t &id);
    };
}

#endif
