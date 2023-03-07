#include "core/net_address.h"
#include "core/socket.h"

int main(){
    libee::NetAddress n1{"1.1.1.1",45};
    std::cout<<"ip "<<n1.GetIP()<<"port "<<n1.GetPort()<<std::endl;
    libee::Socket s1{78};
    std::cout<<"fd "<<s1.GetFd()<<"attr "<<s1.GetAttrs()<<std::endl;
}
