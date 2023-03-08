#include "core/connection.h"
#include "core/net_address.h"
#include "core/socket.h"
#include "core/buffer.h"
#include <memory>

int main(){
    libee::NetAddress n1{"1.1.1.1",45};
    std::cout<<"ip "<<n1.GetIP()<<" port "<<n1.GetPort()<<std::endl;
    libee::Socket s1{78};
    std::cout<<"fd "<<s1.GetFd()<<" attr "<<s1.GetAttrs()<<std::endl;
    size_t ini=44;
    libee::Buffer b1{ini};
    std::cout<<"size "<<b1.size()<<" cap "<<b1.capacity()<<std::endl;
  //  libee::Connection c1{std::unique_ptr<libee::Socket>{&s1}};
    //std::cout<<"fd "<<c1.GetFd();

}
