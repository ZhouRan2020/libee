/*This is an implementation file implementing the socket, which acts as either the listener or client*/
#include "core/socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <stdexcept>

#include "core/net_address.h"
namespace libee{
    static constexpr int BACKLOG=128;

    Socket& Socket::operator=(Socket&& other)noexcept{
        if(fd_!=-1)close(fd_);
        std::swap(fd_,other.fd_);
        return *this;
    }    
    Socket::~Socket(){
        if(fd_!=-1){
            close(fd_);
            fd_=-1;
        }
    }
    void Socket::Connect(NetAddress& server_address){
        if(fd_==-1){
            CreateByProtocol(server_address.GetProtocol());
        }
        if(connect(fd_,server_address.YieldAddr(),*server_address.YieldAddrLen())==-1){
            throw std::logic_error("Socket:connect() error");
        }
    }

    void Socket::Bind(NetAddress& server_address,bool set_reusable){
        if(fd_==-1) CreateByProtocol(server_address.GetProtocol());
        if(set_reusable) SetReusable();
        if(bind(fd_,server_address.YieldAddr(),*server_address.YieldAddrLen())==-1)
        throw std::logic_error("Socket:Bind() error");
    }

    void Socket::Listen(){
        assert(fd_!=-1&&"cannot Listen() with an invalid fd");
        if(listen(fd_,BACKLOG)==-1) throw std::logic_error("Socket Listen() error ");
    }

    int Socket::Accept(NetAddress& client_address){
        assert(fd_!=-1 && "cannot accept() with an invalid fd");
        int client_fd = accept(fd_,client_address.YieldAddr(),client_address.YieldAddrLen());
        if(client_fd==-1);
        return client_fd;
    }

    void Socket::SetReusable(){
        assert(fd_!=-1&&"cannot Setreusable() with an invalid fd");
        int yes=1;
        if(setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes))==-1||
        setsockopt(fd_,SOL_SOCKET,SO_REUSEPORT,&yes,sizeof(yes))==-1)
        throw std::logic_error("Socket:SetReusable() error ");
    }
    void Socket::SetNonBlocking(){
        assert(fd_!=-1 && "cannot SetNonblocking() with an invalid fd");
        if(fcntl(fd_,F_SETFL,fcntl(fd_,F_GETFL))|O_NONBLOCK==-1 ) throw std::logic_error("SOcket: setNOnblocking() error");
    } 
    int Socket::GetAttrs(){
        assert(fd_!=-1&&"cannot Getattrs()with an invalid fd");
        return fcntl(fd_,F_GETFL);
    }
    void Socket::CreateByProtocol(Protocol protocol){
        if(protocol==Protocol::IPv4) fd_=socket(AF_INET,SOCK_STREAM,0);
        else fd_=socket(AF_INET6,SOCK_STREAM,0);
        if(fd_==-1) throw std::logic_error("SOCket:socket() error ");
    }
}