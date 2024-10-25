#include "Download.h"

Download::Download(boost::asio::ip::tcp::socket *socket, HttpMessage &&msg)
    : BaseCgi(socket, std::move(msg)) {}

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
