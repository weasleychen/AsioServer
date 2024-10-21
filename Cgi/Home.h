#ifndef AsioServer_Cgi_Home
#define AsioServer_Cgi_Home

#include "Cgi/BaseCgi.h"

class Home: public BaseCgi {
public:
    Home(boost::asio::ip::tcp::socket *argSocket, const HttpMessage &argMsg);

    int Execute() override;
};

#endif //AsioServer_Cgi_Home
