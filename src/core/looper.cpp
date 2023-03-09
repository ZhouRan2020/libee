#include "core/looper.h"
#include "core/acceptor.h"
#include "core/connection.h"
#include "core/poller.h"
#include "core/thread_pool.h"
#include <memory>
#include <mutex>

namespace libee{
    Looper::Looper():poller_(std::make_unique<Poller>()){}

    void Looper::Loop(){
        while(!exit_){
            auto ready_connections=poller_->Poll(TIMEOUT);
            for(auto& conn:ready_connections)conn->GetCallback();
        }
    }

    void Looper::AddAcceptor(Connection* acceptor_conn){
        std::unique_lock<std::mutex> lock(mtx_);
        poller_->AddConnection(acceptor_conn);
    }

    void Looper::AddConnection(std::unique_ptr<Connection> new_conn){
        std::unique_lock<std::mutex> lock(mtx_);
        poller_->AddConnection(new_conn.get());
        int fd = new_conn->GetFd();
        connections_.insert({fd,std::move(new_conn)});
    }

    bool Looper::DeleteConnection(int fd){
        std::unique_lock<std::mutex> lock(mtx_);
        auto it = connections_.find(fd);
        if(it==connections_.end()) return false;
        connections_.erase(it);
        return true;
    }


}