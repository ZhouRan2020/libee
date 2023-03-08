/* this is a header file implementing the acceptor that accepts all the incoming new client connections and set up customer handle
functions for new clients*/
#ifndef SRC_INCLUDE_CORE_ACCEPTOR_H
#define SRC_INCLUDE_CORE_ACCEPTOR_H
#include <functional>
#include <memory>
#include <vector>

#include "core/connection.h"
#include "core/net_address.h"
#include "core/utils.h"

namespace libee{
    class NetAddress;
    class Looper;
    class Connection;
    /*this acceptor comes with basic functioality for accepting new client connections and distribute its into 
    the different poller.
    more custom handling could be added as well*/
    class Acceptor{
        public:
        Acceptor(Looper* listener,std::vector<Looper*> reactors,NetAddress server_address);
        ~Acceptor() =default;
        NON_COPYABLE(Acceptor);
        void BaseAcceptCallback(Connection* server_conn);
        void SetCustomAcceptCallback(std::function<void(Connection*)> custom_accept_callback);
        void SetCustomHandleCallback(std::function<void(Connection*)> custom_handle_callback);
    
        std::function<void(Connection*)> GetCustomAcceptCallback() const noexcept;
        std::function<void(Connection*)> GetCustomHandleCallback() const noexcept;

        Connection* GetAcceptorConnection() noexcept;

        private:
        std::vector<Looper*> reactors_;
        std::unique_ptr<Connection> acceptor_conn;
        std::function<void(Connection*)> custom_accept_callback_{};
        std::function<void(Connection*)> custom_handle_callback_{};
    };

}
#endif