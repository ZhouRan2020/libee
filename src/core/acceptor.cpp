#include "core/acceptor.h"

#include <cstdlib>
#include <memory>
#include <utility>
#include <vector>

#include "core/connection.h"
#include "core/net_address.h"
#include "core/looper.h"
#include "core/poller.h"
#include "core/socket.h"

namespace libee{
    Acceptor::Acceptor(Looper* listener,std::vector<Looper*> reactors,NetAddress server_address)
        :reactors_(std::move(reactors)){
        auto acceptor_sock=std::make_unique<Socket>();
        acceptor_sock->Bind(server_address,true);
        acceptor_sock->Listen();

        acceptor_conn=std::make_unique<Connection>(std::move(acceptor_sock));
        acceptor_conn->SetEvents(POLL_READ);
        acceptor_conn->SetLooper(listener);

        listener->AddAcceptor(acceptor_conn.get());
        SetCustomAcceptCallback([](Connection*){});
        SetCustomHandleCallback([](Connection*){});
    }

    void Acceptor::BaseAcceptCallback(Connection* server_conn){
        
        NetAddress client_address;
        int accept_fd = server_conn->GetSocket()->Accept(client_address);
        std::cout<<"accepted\n";
        if(accept_fd==-1)return;

        auto client_sock = std::make_unique<Socket>(accept_fd);
        client_sock->SetNonBlocking();

        auto client_connection=std::make_unique<Connection>(std::move(client_sock));
        client_connection->SetEvents(POLL_READ| POLL_ET);
        client_connection->SetCallback(GetCustomHandleCallback());

        int idx=rand()%reactors_.size();
        client_connection->SetLooper(reactors_[idx]);
        reactors_[idx]->AddConnection(std::move(client_connection));
    }
}

