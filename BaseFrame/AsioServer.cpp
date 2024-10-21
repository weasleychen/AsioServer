#include "AsioServer.h"
#include "Log.h"

#include <thread>

AsioServer::AsioServer(int port, int threadCount, HttpCallBack_t cb) :
        handlerCtx(threadCount),
        acceptor(acceptCtx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        mt(std::random_device{}()),
        httpParser(std::move(cb))
{
    RERROR("Config:\n"
           "ThreadCount: {}\n"
           "RunningPort: {}\n", threadCount, port);
}


void AsioServer::Start() {
    StartAccept();

    auto StartEventLoop = [](boost::asio::io_context &ctx) {
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard = boost::asio::make_work_guard(ctx);
        ctx.run();
    };

    std::vector<std::thread> handlerThread(handlerCtx.size());
    for(int i = 0; i < handlerCtx.size(); i++) {
        handlerThread[i] = std::thread(StartEventLoop, std::ref(handlerCtx[i]));
    }

    acceptCtx.run();
}

void AsioServer::StartAccept() {
    int bindCtxIdx = mt() % handlerCtx.size();
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(handlerCtx[bindCtxIdx]);
    acceptor.async_accept(*socket, [this, socket](const boost::system::error_code &ec) {
        if(!ec) {
            this->HandleAccept(socket);
            this->StartAccept();
        }
    });
}

void AsioServer::HandleAccept(const std::shared_ptr<boost::asio::ip::tcp::socket> &socket) {
    RERROR("accept remote {}:{}",
           socket->remote_endpoint().address().to_string(),
           socket->remote_endpoint().port());

    StartRead(socket);
}

void AsioServer::StartRead(const std::shared_ptr<boost::asio::ip::tcp::socket> &socket) {
    auto buffer = std::make_shared<std::string>(1024, '\0');
    socket->async_read_some(boost::asio::buffer(*buffer),
        [this, buffer, socket](const boost::system::error_code &ec, std::size_t bufferSize) {
            if(!ec) {
                this->HandleRead(socket, buffer->substr(0, bufferSize));
                this->StartRead(socket);
            } else {
                if(ec == boost::asio::error::eof ||
                    ec == boost::asio::error::connection_reset)
                {
                    httpParser.Cancel(&*socket);
                }
            }
        });
}

void AsioServer::HandleRead(const std::shared_ptr<boost::asio::ip::tcp::socket> &socket, const std::string &msg) {
    RERROR("Feed: {}", msg);
    this->httpParser.Feed(socket.get(), msg);
}
