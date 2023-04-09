#include "../../consts.h"
#include <fin_db/db.h>
#include <fin_db/utils.h>
#include <string>

fin::DB::DB() {
    //Set up the account
    mariadb::account_ref acc = mariadb::account::create(
            consts::db::HOSTNAME,
            consts::db::USERNAME,
            consts::db::PASSWORD
    );

    //Create connection
    this->conn = mariadb::connection::create(acc);

    //Create the database if it doesn't exist
    if (!exists())
        create();

    //Prepare the statements
    this->insSongInfoStmt = this->conn->create_statement(
            "INSERT INTO " + consts::db::INFO_TABLE_FULL + " (name) VALUES (?)");
    this->selSongById = this->conn->create_statement(
            "SELECT name FROM " + consts::db::INFO_TABLE_FULL + " WHERE id = ?");
}

bool fin::DB::exists() {
    try {
        mariadb::result_set_ref result = this->conn->query(
                "SELECT table_name FROM information_schema.tables WHERE table_schema='" + consts::db::NAME + "' " +
                "OR table_name='" + consts::db::RECORDINGS_TABLE + "' " +
                "OR table_name='" + consts::db::INFO_TABLE + "' " +
                "OR table_name='" + consts::db::TMP_RECORD_TABLE + "'");

        return result->row_count() == 3;
    } catch (const std::exception &e) {}

    return false;
}

bool fin::DB::create() {
    try {
        this->conn->execute("CREATE DATABASE IF NOT EXISTS " + consts::db::NAME);

        this->conn->execute("CREATE TABLE IF NOT EXISTS " + consts::db::INFO_TABLE_FULL + " (" +
                            "id " + consts::db::UINT + " NOT NULL AUTO_INCREMENT PRIMARY KEY," +
                            "name VARCHAR(200) NOT NULL)");

        this->conn->execute("CREATE TABLE IF NOT EXISTS " + consts::db::RECORDINGS_TABLE_FULL + " (" +
                            "hash " + consts::db::UINT64 + " NOT NULL, " +
                            "songId " + consts::db::UINT + " NOT NULL, " +
                            "time " + consts::db::UINT64 + " NOT NULL, " +
                            "PRIMARY KEY (hash, songId, time), " +
                            "FOREIGN KEY (songId) REFERENCES " + consts::db::INFO_TABLE_FULL + "(id))");

    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    return true;
}

bool fin::DB::drop() {
    try {
        this->conn->execute("DROP DATABASE IF EXISTS " + consts::db::NAME);
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    return true;
}

bool fin::DB::insertSong(const std::string &name, const fin::core::Links &links) {
    mariadb::u64 id;

    //Insert song details into INFO_TABLE
    try {
        this->insSongInfoStmt->set_string(0, name);
        id = this->insSongInfoStmt->insert(); //PK, id of the song
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    //Insert links
    std::string s = "INSERT INTO " + consts::db::RECORDINGS_TABLE_FULL + " (hash, songId, time) VALUES ";

    for (const auto &link : links) {
        s += "(";
        s += utils::toHex(link.getHash()); //hex for speed purposes
        s += ",";
        s += utils::toHex(id);
        s += ",";
        s += utils::toHex(link.getTime());
        s += "),";
    }

    s.pop_back(); //Remove last (useless) ","

    try {
        conn->insert(s);
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    return true;
}

std::string fin::DB::getSongNameById(const std::uint64_t &id) {
    mariadb::result_set_ref result;

    try {
        this->selSongById->set_unsigned64(0, id);
        result = this->selSongById->query();
    } catch (const std::exception &e) {
        return "";
    }

    if (result->next())
        return result->get_string("name");
    else
        return "";
}

bool fin::DB::searchIdGivenLinks(const fin::core::Links &links, std::uint64_t &id, std::uint64_t *commonLinks) {
    //Create the temporary in memory table
    try {
        this->conn->execute("CREATE TEMPORARY TABLE " + consts::db::TMP_RECORD_TABLE_FULL + " (" +
                            "hash " + consts::db::UINT64 + " NOT NULL, " +
                            "start " + consts::db::UINT64 + " NOT NULL, " +
                            "PRIMARY KEY (hash, start)" +
                            ") ENGINE = MEMORY");
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    //Insert recording links in the temporary table
    std::string s = "INSERT INTO " + consts::db::TMP_RECORD_TABLE_FULL + " VALUES ";

    for (const auto &link : links) {
        s += "(";
        s += utils::toHex(link.getHash());
        s += ",";
        s += utils::toHex(link.getTime());
        s += "),";
    }

    s.pop_back(); //Remove last (useless) ","

    try {
        conn->insert(s);
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    //Perform the real query to find similarities
    mariadb::result_set_ref result;

    try {
        result = this->conn->query(
                //selected songId, number of common links
                "SELECT " + consts::db::RECORDINGS_TABLE_FULL + ".songId, COUNT(*) AS n " +
                //inner join the songs table and the temporary table
                "FROM " + consts::db::RECORDINGS_TABLE_FULL + " INNER JOIN " + consts::db::TMP_RECORD_TABLE_FULL + " " +
                //join condition: the hash has to be the same
                "ON " + consts::db::RECORDINGS_TABLE_FULL + ".hash=" + consts::db::TMP_RECORD_TABLE_FULL + ".hash " +
                //since the recording is a piece of the full song, so the whole recording has to be shifted of a non
                //negative amount of time w.r.t. to original song
                "WHERE " + consts::db::RECORDINGS_TABLE_FULL + ".time>=" + consts::db::TMP_RECORD_TABLE_FULL + ".start " +
                //the common links are grouped if the time difference between the original song links and the recording
                //ones is the same
                "GROUP BY " + consts::db::RECORDINGS_TABLE_FULL + ".time-" + consts::db::TMP_RECORD_TABLE_FULL +
                ".start, songId " +
                //the more links in common the better
                "ORDER BY n DESC");

        //found something && is it above the minimum threshold
        if (result->next() && result->get_unsigned64(1) > consts::links::MIN_HINT)
            id = result->get_unsigned32(0);

        //should we return the number of common links?
        if (commonLinks != nullptr)
            *commonLinks = result->get_unsigned64(1);

    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    try {
        conn->execute("DROP TABLE " + consts::db::TMP_RECORD_TABLE_FULL);
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    return true;
}

fin::DB::~DB() {
    this->conn->disconnect();
}
