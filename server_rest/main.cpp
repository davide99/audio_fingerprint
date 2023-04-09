#include <iostream>
#include <httplib.h>
#include <fin/utils/byte_buffer.h>
#include <fin/core/links.h>

int main() {
    httplib::Server svr;

    svr.Post("/songByLinks",
             [&](
                     const httplib::Request &req, httplib::Response &res,
                     const httplib::ContentReader &content_reader) {
                 if (req.get_header_value("Content-Type") == "application/octet-stream") {
                     content_reader([&](const char *data, size_t data_length) {
                         fin::utils::ByteBuffer buffer;

                         for (size_t i = 0; i < data_length; i++) {
                             buffer.add8(static_cast<std::uint8_t>(data[i]));
                         }

                         fin::core::Links links = fin::core::Links::fromByteBuffer(buffer);

                         return true;
                     });

                     res.set_header("Access-Control-Allow-Origin", req.get_header_value("Origin"));
                     res.set_header("Allow", "POST, OPTIONS");
                     res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Origin, Authorization");
                     res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
                     res.set_content("{'ok': 'a'}", "application/json");
                 } else {
                     res.set_content("{'message': 'no'}", "application/json");
                     res.status = 500;
                 }
             });

    svr.Options("/songByLinks", [](const auto& req, auto& res) {
        res.set_header("Access-Control-Allow-Origin", req.get_header_value("Origin"));
        res.set_header("Allow", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Origin, Authorization");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    });
    svr.listen("0.0.0.0", 8080);

    return 0;
}
