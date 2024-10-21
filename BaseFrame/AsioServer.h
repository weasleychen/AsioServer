#ifndef AsioServer_Include_AsioServer
#define AsioServer_Include_AsioServer

#include <vector>
#include <random>

#include "boost/asio.hpp"
#include "HttpParser.h"

class AsioServer {
public:
    using HttpCallBack_t = HttpParser<boost::asio::ip::tcp::socket*>::CallBack_t;
    AsioServer(int port, int threadCount, HttpCallBack_t cb);

    void Start();

    void StartAccept();
    void HandleAccept(const std::shared_ptr<boost::asio::ip::tcp::socket> &socket);
    void StartRead(const std::shared_ptr<boost::asio::ip::tcp::socket> &socket);
    void HandleRead(const std::shared_ptr<boost::asio::ip::tcp::socket> &socket, const std::string &msg);
private:

    boost::asio::io_context acceptCtx;
    boost::asio::ip::tcp::acceptor acceptor;
    std::vector<boost::asio::io_context> handlerCtx;
    std::mt19937 mt;
    HttpParser<boost::asio::ip::tcp::socket*> httpParser;
};


#endif
