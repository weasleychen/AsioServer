#include <fstream>
#include <iostream>

#include "Cgi/BaseCgi.h"

BaseCgi::BaseCgi(boost::asio::ip::tcp::socket *argSocket, HttpMessage &&argMsg)
    : socket(argSocket), msg(std::move(argMsg)) {}

std::size_t BaseCgi::WriteResponse(const std::string &body, std::map<std::string, std::string> headers) {
    static std::map<std::string, std::string> defaultHeaders = {
        {"Content-Type", "text/plain"},
    };

    int ret = 0;

    std::string response = std::string("HTTP/1.1 200 OK\r\n")
                           + AccumulateHttpHeadersAsString(MergeMap(defaultHeaders, headers))
                           + std::format("Content-Length: {}\r\n", body.size())
                           + "\r\n"
                           + body;

    std::size_t writeDone = boost::asio::write(*socket, boost::asio::buffer(response));
    if(writeDone != response.size()) {
        UERROR("boost::asio::write error, writeDone: {}", writeDone);
        return 1;
    }

    return 0;
}
