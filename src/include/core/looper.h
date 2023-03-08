#ifndef SRC_INCLUDE_CORE_LOOPER_H
#define SRC_INCLUDE_CORE_LOOPER_H

#include <atomic>
#include <functional>
#include <future>
#include <map>
#include <memory> 
#include <mutex>

#include "core/acceptor.h"
#include "core/connection.h"
#include "core/utils.h"

namespace libee {
    static constexpr int TIMEOUT = 3000;
    class Poller;
    class ThreadPool;
    class Connection;
    class Acceptor;
    class Looper{
        public:
        Looper();
        ~Looper()=default;
        NON_COPYABLE(Looper);
        void Loop();
        void AddAcceptor(Connection* acceptor_conn);
        void AddConnection(std::unique_ptr<Connection> new_conn);
        bool DeleteConnection(int fd);
        void SetExit() noexcept;
        private:
        std::unique_ptr<Poller> poller_;
        std::mutex mtx_;
        std::map<int,std::unique_ptr<Connection>> connections_;
        bool exit_{false};
    };
}

#endif