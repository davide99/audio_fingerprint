#include "../consts.h"
#include <iostream>
#include <httplib.h>
#include <fin/utils/byte_buffer.h>
#include <fin/core/links.h>
#include <fin_db/fin_db.h>

int main() {
    httplib::Server svr;
    fin::DB db;

    svr.Post(consts::rest::SEARCH_ENDPOINT,
             [&](
                     const auto &req, auto &res,
                     const auto &content_reader) {
                 if (req.get_header_value("Content-Type") == consts::rest::CONTENT_TYPE_BINARY) {
                     fin::utils::ByteBuffer buffer;

                     content_reader([&](const char *data, std::size_t data_length) {
                         for (decltype(data_length) i = 0; i < data_length; i++)
                             buffer.add8(static_cast<std::uint8_t>(data[i]));

                         return true;
                     });

                     fin::core::Links links = fin::core::Links::fromByteBuffer(buffer);
                     std::cout << buffer.getSize() << " bytes received, "
                               << links.size() << " links received" << std::endl;

                     std::string song = fin::searchFromLinks(links, db);
                     if (!song.empty()) {
                         res.set_content(R"({"song_name": ")" + song + "\"}", consts::rest::CONTENT_TYPE_JSON);
                     } else {
                         res.set_content(R"({"error": "song not found"})", consts::rest::CONTENT_TYPE_JSON);
                         res.status = 404;
                     }
                 } else {
                     res.set_content(R"({"error": "wrong Content-Type"})", consts::rest::CONTENT_TYPE_JSON);
                     res.status = 415;
                 }
             });

    svr.set_pre_routing_handler([](const auto &req, auto &res) {
        (void) res;

        if (req.method == "OPTIONS")
            return httplib::Server::HandlerResponse::Handled;

        return httplib::Server::HandlerResponse::Unhandled;
    });

    svr.set_post_routing_handler([](const auto &req, auto &res) {
        res.set_header("Access-Control-Allow-Origin", req.get_header_value("Origin"));
        res.set_header("Allow", "GET, POST, HEAD, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Origin, Authorization");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, HEAD, OPTIONS");
    });

    svr.listen("0.0.0.0", consts::rest::PORT);

    return 0;
}
