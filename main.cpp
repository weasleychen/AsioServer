#include "BaseFrame/Config.h"
#include "BaseFrame/AsioServer.h"
#include "Cgi/AllCgi.h"
#include "BaseFrame/Log.h"

#include <map>

template <typename Cgi>
int ExecuteCgi(boost::asio::ip::tcp::socket *socket, const HttpMessage &msg) {
    int ret = 0;

    Cgi cgi(socket, msg);

    ret = cgi.Execute();
    if(ret != 0) {
        // Log?
        return ret;
    }

    return 0;
}

// transfer to cgi
void HandleHttp(boost::asio::ip::tcp::socket *socket, const HttpMessage &msg) {
    int ret = 0;

    #define WrapCgiFunction(CgiName) [&](){ ExecuteCgi<CgiName>(socket, msg); }
    std::map<std::string, std::function<void()>> mp = {
        {"/", WrapCgiFunction(Home)},
        {"/Echo", WrapCgiFunction(Echo)},
        {"/Pic", WrapCgiFunction(Pic)},
        {"/Download", WrapCgiFunction(Download)},
    };

    if(!mp.contains(msg.requestPath)) {
        WERROR("Unexpected path: {}", msg.requestPath);
        return;
    }

    mp.at(msg.requestPath)();
}

int main(int argc, char **argv) {
    Config config("AsioServer.conf");
    const int kThreadCount = config.Get<int>("ThreadCount", 3);
    const int kPort = config.Get<int>("Port", 80);

    AsioServer server(kPort, kThreadCount, HandleHttp);
    server.Start();

    return 0;
}