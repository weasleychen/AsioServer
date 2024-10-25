#include "Cgi/Echo.h"

#include <format>

Echo::Echo(boost::asio::ip::tcp::socket *argSocket, HttpMessage &&argMsg)
    : BaseCgi(argSocket, std::move(argMsg)) {}

int Echo::Execute() {
    int ret = 0;

    auto action = msg.GetQueryString("action");
    if(action) {
        WERROR("action: {}", action.value());
        if(action == "PlusTwoNumber") {
            std::vector numbers = Split(msg.body, "\\s+");
            if(numbers.size() != 2) {
                std::string responseBody = "Invalid two number!";
                std::string response = std::string("HTTP/1.1 200 OK\r\n")
                                       + "Content-Type: text/plain\r\n"
                                       + std::format("Content-Length: {}\r\n", responseBody.size())
                                       + "\r\n"
                                       + responseBody;

                boost::asio::write(*socket, boost::asio::buffer(response));
                return 0;
            }

            std::vector<int> nums;
            try {
                std::transform(numbers.begin(), numbers.end(), std::back_inserter(nums),
                    [](const std::string &v){
                        return std::stoi(v);
                    });
            } catch(const std::exception &e) {
                std::string responseBody = "Invalid two number!";
                std::string response = std::string("HTTP/1.1 200 OK\r\n")
                                       + "Content-Type: text/plain\r\n"
                                       + std::format("Content-Length: {}\r\n", responseBody.size())
                                       + "\r\n"
                                       + responseBody;

                boost::asio::write(*socket, boost::asio::buffer(response));
                return 0;
            }

            std::string responseBody = std::to_string(nums[0] + nums[1]);
            std::string response = std::string("HTTP/1.1 200 OK\r\n")
                                   + "Content-Type: text/plain\r\n"
                                   + std::format("Content-Length: {}\r\n", responseBody.size())
                                   + "\r\n"
                                   + responseBody;

            boost::asio::write(*socket, boost::asio::buffer(response));
            return 0;
        } else if(action == "ToUpper") {
            std::string responseBody = msg.body;
            std::for_each(responseBody.begin(), responseBody.end(),
                [](char &c) { c = static_cast<char>(std::toupper(c)); });

            std::string response = std::string("HTTP/1.1 200 OK\r\n")
                                   + "Content-Type: text/plain\r\n"
                                   + std::format("Content-Length: {}\r\n", responseBody.size())
                                   + "\r\n"
                                   + responseBody;

            boost::asio::write(*socket, boost::asio::buffer(response));
        }
    }

    std::string responseBody;
    if(msg.body.empty()) {
        responseBody = "Hello, What's wrong?";
    } else {
        responseBody = msg.body;
    }

    std::string response = std::string("HTTP/1.1 200 OK\r\n")
                           + "Content-Type: text/plain\r\n"
                           + std::format("Content-Length: {}\r\n", responseBody.size())
                           + "\r\n"
                           + responseBody;

    boost::asio::write(*socket, boost::asio::buffer(response));
    return 0;
}
