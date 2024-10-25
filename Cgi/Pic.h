#ifndef AsioSever_Cgi_Pic
#define AsioSever_Cgi_Pic

#include "Cgi/BaseCgi.h"

class Pic: public BaseCgi {
public:
    Pic(boost::asio::ip::tcp::socket *argSocket, HttpMessage &&argMsg);

    int Execute() override;
};


#endif //AsioSever_Cgi_Pic
