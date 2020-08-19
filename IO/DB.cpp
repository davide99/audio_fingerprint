#include "DB.h"
#include <string>
#include "../Math/Integers.h"

IO::DB::DB() {
    //Setup the account
    mariadb::account_ref acc = mariadb::account::create(
            Consts::DB::Hostname,
            Consts::DB::Username,
            Consts::DB::Password
    );

    //Create connection
    this->conn = mariadb::connection::create(acc);

    //Create the database if it doesn't exist
    if (!exists())
        create();

    //Prepare the statements
    this->insSongInfoStmt = this->conn->create_statement(
            "INSERT INTO " + Consts::DB::InfoTableFull + " (name) VALUES (?)");
    this->selSongById = this->conn->create_statement(
            "SELECT name FROM " + Consts::DB::InfoTableFull + " WHERE id = ?");
}

bool IO::DB::exists() {
    try {
        mariadb::result_set_ref result = this->conn->query(
                "SELECT table_name FROM information_schema.tables WHERE table_schema='" + Consts::DB::Name + "' " +
                "OR table_name='" + Consts::DB::RecordingsTable + "' " +
                "OR table_name='" + Consts::DB::InfoTable + "' " +
                "OR table_name='" + Consts::DB::TmpRecordTable + "'");

        return result->row_count() == 3;
    } catch (const std::exception &e) {}

    return false;
}

bool IO::DB::create() {
    try {
        this->conn->execute("CREATE DATABASE IF NOT EXISTS " + Consts::DB::Name);

        this->conn->execute("CREATE TABLE IF NOT EXISTS " + Consts::DB::InfoTableFull + " (" +
                            "id " + Consts::DB::UInt + " NOT NULL AUTO_INCREMENT PRIMARY KEY," +
                            "name VARCHAR(200) NOT NULL)");

        this->conn->execute("CREATE TABLE IF NOT EXISTS " + Consts::DB::RecordingsTableFull + " (" +
                            "hash " + Consts::DB::UInt64 + " NOT NULL, " +
                            "songId " + Consts::DB::UInt + " NOT NULL, " +
                            "time " + Consts::DB::UInt64 + " NOT NULL, " +
                            "PRIMARY KEY (hash, songId, time), " +
                            "FOREIGN KEY (songId) REFERENCES " + Consts::DB::InfoTableFull + "(id))");

    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    return true;
}

bool IO::DB::drop() {
    try {
        this->conn->execute("DROP DATABASE IF EXISTS " + Consts::DB::Name);
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    return true;
}

bool IO::DB::insertSong(const std::string &name, const Core::Links &links) {
    mariadb::u64 id;

    //Insert song details into InfoTable
    try {
        this->insSongInfoStmt->set_string(0, name);
        id = this->insSongInfoStmt->insert(); //PK, id of the song
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    //Insert Links
    std::string s = "INSERT INTO " + Consts::DB::RecordingsTableFull + " (hash, songId, time) VALUES ";

    for (const auto &link : links) {
        s += "(";
        s += Math::Integers::toHex(link.getHash()); //hex for speed purposes
        s += ",";
        s += Math::Integers::toHex(id);
        s += ",";
        s += Math::Integers::toHex(link.getTime());
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

std::string IO::DB::getSongNameById(const std::uint64_t &id) {
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

bool IO::DB::searchIdGivenLinks(const Core::Links &links, std::uint64_t &id, std::uint64_t *commonLinks) {
    //Create the temporary in memory table
    try {
        this->conn->execute("CREATE TEMPORARY TABLE " + Consts::DB::TmpRecordTableFull + " (" +
                            "hash " + Consts::DB::UInt64 + " NOT NULL, " +
                            "start " + Consts::DB::UInt64 + " NOT NULL, " +
                            "PRIMARY KEY (hash, start)" +
                            ") ENGINE = MEMORY");
    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    //Insert recording links in the temporary table
    std::string s = "INSERT INTO " + Consts::DB::TmpRecordTableFull + " VALUES ";

    for (const auto &link : links) {
        s += "(";
        s += Math::Integers::toHex(link.getHash());
        s += ",";
        s += Math::Integers::toHex(link.getTime());
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
                "SELECT " + Consts::DB::RecordingsTableFull + ".songId, COUNT(*) AS n " +
                //inner join the songs table and the temporary table
                "FROM " + Consts::DB::RecordingsTableFull + " INNER JOIN " + Consts::DB::TmpRecordTableFull + " " +
                //join condition: the hash has to be the same
                "ON " + Consts::DB::RecordingsTableFull + ".hash=" + Consts::DB::TmpRecordTableFull + ".hash " +
                //since the recording is a piece of the full song, so the whole recording has to be shifted of a non
                //negative amount of time w.r.t. to original song
                "WHERE " + Consts::DB::RecordingsTableFull + ".time>=" + Consts::DB::TmpRecordTableFull + ".start " +
                //the common links are grouped if the time difference between the original song links and the recording
                //ones is the same
                "GROUP BY " + Consts::DB::RecordingsTableFull + ".time-" + Consts::DB::TmpRecordTableFull +
                ".start, songId " +
                //the more links in common the better
                "ORDER BY n DESC");

        //found something && is it above the minimum threshold
        if (result->next() && result->get_unsigned64(1) > Consts::Links::MinHint)
            id = result->get_unsigned32(0);

        //should we return the number of common links?
        if (commonLinks != nullptr)
            *commonLinks = result->get_unsigned64(1);

    } catch (const std::exception &e) {
        e.what();
        return false;
    }

    return true;
}

IO::DB::~DB() {
    this->conn->disconnect();
}
