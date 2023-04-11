#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "core/acceptor.h"
#include "core/buffer.h"
#include "core/connection.h"
#include "core/looper.h"
#include "core/net_address.h"
#include "core/poller.h"
#include "core/socket.h"
#include "core/thread_pool.h"
#include "core/utils.h"

#ifndef SRC_INCLUDE_CORE_LIBEE_SERVER_H
#define SRC_INCLUDE_CORE_LIBEE_SERVER_H
namespace libee{
    class Server{
        public:
            Server(NetAddress server_address,int concurrency=static_cast<int>(std::thread::hardware_concurrency())-1)
            :_pool(std::make_unique<ThreadPool>(concurrency)),_listener(std::make_unique<Looper>()){
                for(size_t i=0;i<_pool->GetSize();++i)
                    _reactors.push_back(std::make_unique<Looper>());
                for(auto& reactor:_reactors)
                    _pool->SubmitTask([capture0=reactor.get()]{capture0->Loop();});
                std::vector<Looper*> raw_reactors;
                raw_reactors.reserve(_reactors.size());
                std::transform(_reactors.begin(),_reactors.end(),std::back_inserter(raw_reactors),
                            [](auto& uni_ptr){return uni_ptr.get();});
                _acceptor=std::make_unique<Acceptor>(_listener.get(),raw_reactors,server_address);
            }




            virtual ~Server()=default;
            Server& OnAccept(std::function<void(Connection*)> on_accept){
                _acceptor->SetCustomAcceptCallback(std::move(on_accept));
                return *this;
            }

            Server& OnHandle(std::function<void(Connection*)> on_handle){
                _acceptor->SetCustomHandleCallback(std::move(on_handle));
                _on_handle_set=true;
                return *this;
            }
            void Begin(){
                if(!_on_handle_set)
                    throw std::logic_error("please specify onhandle callback before starts");
                _listener->Loop();
            }
        private:
            bool _on_handle_set{false};
            std::unique_ptr<Acceptor> _acceptor;
            std::vector<std::unique_ptr<Looper>> _reactors;
            std::unique_ptr<ThreadPool> _pool;
            std::unique_ptr<Looper> _listener;
    };
}
#endif