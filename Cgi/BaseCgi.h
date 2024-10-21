#ifndef AsioServer_Cgi_BaseCgi
#define AsioServer_Cgi_BaseCgi

#include <map>

#include "boost/asio.hpp"
#include "BaseFrame/HttpParser.h"
#include "HttpHeaderHelper.h"

class BaseCgi {
public:
    BaseCgi(boost::asio::ip::tcp::socket *argSocket, const HttpMessage &argMsg);

    std::size_t WriteResponse(const std::string &body, std::map<std::string, std::string> headers = {});

    virtual int Execute() = 0;
protected:
    boost::asio::ip::tcp::socket *socket;
    const HttpMessage &msg;
};

#endif // AsioServer_Cgi_BaseCgi
