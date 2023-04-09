#include <iostream>
#include <httplib.h>
#include <fin/utils/byte_buffer.h>
#include <fin/core/links.h>
#include <fin_db/fin_db.h>

int main() {
    httplib::Server svr;
    fin::DB db;

    svr.Post("/songByLinks",
             [&](
                     const httplib::Request &req, httplib::Response &res,
                     const httplib::ContentReader &content_reader) {
                 if (req.get_header_value("Content-Type") == "application/octet-stream") {
                     fin::utils::ByteBuffer buffer;

                     content_reader([&](const char *data, size_t data_length) {
                         for (size_t i = 0; i < data_length; i++) {
                             buffer.add8(static_cast<std::uint8_t>(data[i]));
                         }

                         return true;
                     });

                     fin::core::Links links = fin::core::Links::fromByteBuffer(buffer);
                     std::cout << "Ricevuti " << buffer.getSize() << " bytes, " << links.size() << " Link" << std::endl;
                     std::string song = fin::searchFromLinks(links, db);

                     res.set_content("{\"song\": \"" + song + "\"}", "application/json");
                 } else {
                     res.set_content(R"({"message": "no"})", "application/json");
                     res.status = 500;
                 }
             });

    svr.set_pre_routing_handler([](const auto &req, auto &res) {
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

    svr.listen("0.0.0.0", 8080);

    return 0;
}
