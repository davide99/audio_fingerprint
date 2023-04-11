#ifndef FIN_DB_DB_H
#define FIN_DB_DB_H

#include <mariadb++/connection.hpp>
#include <mariadb++/statement.hpp>
#include <cstdint>
#include <fin/core/links.h>
#include <tuple>

/**
 * Class to manage the database
 */
namespace fin {
    class DB {
    private:
        mariadb::connection_ref conn_;
        mariadb::statement_ref insSongInfoStmt_, selSongById_;

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
        void create();

        /**
         * Insert a song in the database
         * @param name  NAME of the song
         * @param links Song's links
         * @return true if no errors occurred, false otherwise
         */
        bool insertSong(const std::string &name, const fin::core::Links &links);

        /**
         * Try to find a match for a recording
         * @param links         Recoding's links
         * @return the song id, 0 if not found
         */
        std::uint64_t searchIdGivenLinks(const fin::core::Links &links);

        /**
         * Find the name of a song
         * @param id Id of the song
         * @return The name if found, an empty string otherwise
         */
        std::string getSongNameById(const std::uint64_t &id);
    };
}

#endif
