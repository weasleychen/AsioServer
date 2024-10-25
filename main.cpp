#include "BaseFrame/Config.h"
#include "BaseFrame/AsioServer.h"
#include "Cgi/AllCgi.h"
#include "BaseFrame/Log.h"

#include <map>
#include <utility>

template <typename Cgi>
int ExecuteCgi(boost::asio::ip::tcp::socket *socket, HttpMessage &msg) {
    int ret = 0;

    Cgi cgi(socket, std::move(msg));

    ret = cgi.Execute();
    if(ret != 0) {
        // Log?
        return ret;
    }

    return 0;
}

// transfer to cgi
void HandleHttp(boost::asio::ip::tcp::socket *socket, HttpMessage &msg) {
    int ret = 0;

    using ExectutorType = std::function<int(boost::asio::ip::tcp::socket *socket, HttpMessage &msg)>;
    static const std::map<std::string, ExectutorType> toExecutor = {
        {"/", ExecuteCgi<Home>},
        {"/Echo", ExecuteCgi<Echo>},
        {"/Pic", ExecuteCgi<Pic>},
        {"/Download", ExecuteCgi<Download>},
    };

    if(!toExecutor.contains(msg.requestPath)) {
        WERROR("Unexpected path: {}", msg.requestPath);
        return;
    }

    toExecutor.at(msg.requestPath)(socket, msg);
}

int main(int argc, char **argv) {
    Config config("AsioServer.conf");
    const int kThreadCount = config.Get<int>("ThreadCount", 3);
    const int kPort = config.Get<int>("Port", 80);

    AsioServer server(kPort, kThreadCount, HandleHttp);
    server.Start();

    return 0;
}