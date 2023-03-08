#include "core/connection.h"
#include "core/poller.h"
#include "core/socket.h"
#include <iostream>
#include <memory>
#include <ostream>

int main(){
    //std::unique_ptr<libee::Socket> p1{ new libee::Socket{78} };
    libee::Connection c1{std::unique_ptr<libee::Socket>{new libee::Socket{78}}};
    std::cout<<c1.GetFd()<<std::endl;
    libee::Poller p1{};
  //  p1.AddConnection(&c1);
    std::cout<<p1.GetPollSize()<<std::endl;

}