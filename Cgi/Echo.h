#ifndef AsioServer_Cgi_Echo
#define AsioServer_Cgi_Echo

#include "BaseCgi.h"

class Echo: public BaseCgi {
public:
    Echo(boost::asio::ip::tcp::socket *argSocket, const HttpMessage &argMsg);

    int Execute() override;
};


#endif // AsioServer_Cgi_Echo
