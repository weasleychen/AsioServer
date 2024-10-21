#include "Cgi/Home.h"

#include "Utility/Comm.h"

Home::Home(boost::asio::ip::tcp::socket *argSocket, const HttpMessage &argMsg)
    : BaseCgi(argSocket, argMsg) {}

int Home::Execute() {
    int ret = 0;

    std::string html = ReadFile("StaticSource/index.html");

    WriteResponse(html, {{"Content-Type", "text/html"}});

    return 0;
}