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
                         buffer.add(reinterpret_cast<const std::uint8_t *>(data), data_length);
                         return true;
                     });

                     fin::core::Links links = fin::core::Links::fromByteBuffer(buffer);
                     std::cout << buffer.getSize() << " bytes received, "
                               << links.size() << " links received" << std::endl;

                     auto result = fin::searchFromLinks(links, db);
                     if (result.found) {
                         std::string json_result;
                         json_result = "{";
                         json_result += R"("song_name": ")" + result.name + R"(",)";
                         json_result += R"("id": ")" + std::to_string(result.id) + R"(",)";
                         json_result += R"("time_delta": ")" + std::to_string(result.timeDelta) + R"(",)";
                         json_result += R"("common_links": ")" + std::to_string(result.commonLinks) + R"(")";
                         json_result += "}";

                         res.set_content(json_result, consts::rest::CONTENT_TYPE_JSON);
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
