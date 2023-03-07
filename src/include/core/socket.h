/* This is a header file implementing the socket, which acts as either the listener or client*/
#ifndef SRC_INCLUDE_CORE_SOCKET_H
#define SRC_INCLUDE_CORE_SOCKET_H
#include "core/utils.h"

namespace libee{
    enum class Protocol;
    class NetAddress;
    /*This Socket class encapsulates a socket descriptor which can act as either listener or client.
    The class is compatiable with both ipv4 or ipv6 */\
    class Socket{

    public:
        Socket() noexcept =default;
        Socket(int fd) noexcept : fd_{fd}{}
        NON_COPYABLE(Socket);
        Socket(Socket&& other)noexcept{
            fd_=other.fd_;
            other.fd_=-1;
        }
        Socket& operator= (Socket&& other) noexcept;
       

        ~Socket();

        int GetFd() const noexcept{return fd_;}
        /*for client, one step:directly connect*/
        void Connect(NetAddress& server_address);
        /*for server, bind+listen+accept*/
        void Bind(NetAddress& server_address,bool set_reusable=true);
        void Listen();
        int Accept(NetAddress& client_address);
        void SetReusable();
        void SetNonBlocking();
        int GetAttrs();

    private:

        void CreateByProtocol(Protocol Protocol);
        int fd_{-1};
    };
    
}

#endif