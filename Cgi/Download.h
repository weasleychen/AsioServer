#ifndef AsioServer_Cgi_Download
#define AsioServer_Cgi_Download

#include "BaseCgi.h"

class Download: public BaseCgi {
public:
    Download(boost::asio::ip::tcp::socket *socket, const HttpMessage &msg);

    int Execute() override;
};


#endif // AsioServer_Cgi_Download
