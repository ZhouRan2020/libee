#include "core/net_address.h"

int main(){
    libee::NetAddress n1{"1.1.1.1",45};
    std::cout<<n1.GetIP()<<"port"<<n1.GetPort();
}