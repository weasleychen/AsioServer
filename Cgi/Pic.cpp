#include <format>

#include "Cgi/Pic.h"
#include "Cgi/HttpHeaderHelper.h"

Pic::Pic(boost::asio::ip::tcp::socket *argSocket, const HttpMessage &argMsg)
    : BaseCgi(argSocket, argMsg) {}

int Pic::Execute() {
    int ret = 0;

    auto picName = msg.GetQueryString("name");
    if(picName) {
        std::string filePath = std::format("StaticSource/{}.png", picName.value());
        std::string content = ReadFile(filePath);

        if(content.empty()) {
            filePath = std::format("StaticSource/{}.jpg", picName.value());
            content = ReadFile(filePath);
            WriteResponse(content, {kJpgFormat});
        } else {
            WriteResponse(content, {kPngFormat});
        }
    }

    
    return 0;
}

