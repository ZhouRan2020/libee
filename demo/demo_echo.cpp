#include "core/connection.h"
#include "core/net_address.h"
#include "core/server.h"

int main(){
    libee::NetAddress lo("127.0.0.1",1888);
    libee::Server es(lo);
    std::cout<<"ok\n";
    es.OnHandle([&](libee::Connection *client_conn){
        int from_fd=client_conn->GetFd();
        std::cout<<"got fd\n";
        auto [read,exit]=client_conn->Recv();
        std::cout<<"mess recved\n";
        if(exit){
            client_conn->GetLooper()->DeleteConnection(from_fd);
            return;
        }
        if(read){
            client_conn->WriteToWriteBuffer(client_conn->ReadAsString());
            std::cout<<"write to buff\n";
            client_conn->Send();
            std::cout<<"sent done\n";
            client_conn->ClearReadBuffer();
        }
    }).Begin();
}