#include "Download.h"

Download::Download(boost::asio::ip::tcp::socket *socket, const HttpMessage &msg)
    : BaseCgi(socket, msg) {}

int Download::Execute() {
    int ret = 0;

    auto path = msg.GetQueryString("path");
    if(path) {
        std::string content = ReadFile(std::format("StaticSource/{}", path.value()));

        WriteResponse(content, {
            kRawTextFormat,
            SpecifyDownloadFileName(path.value()),
        });
    }

    return 0;
}
