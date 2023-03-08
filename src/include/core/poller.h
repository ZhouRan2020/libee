#ifndef SRC_INCLUDE_CORE_POLLER_H
#define SRC_INCLUDE_CORE_POLLER_H

#include "core/connection.h"
#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#include <memory>
#include <vector>

#include "core/utils.h"

namespace libee{
    static constexpr int DEFAULT_EVENTS_LISTENED = 1024;
    #ifdef OS_LINUX
    static constexpr unsigned POLL_ADD = EPOLL_CTL_ADD;
    static constexpr unsigned POLL_READ = EPOLLIN;
    static constexpr unsigned POLL_ET = EPOLLET;
    #endif

    class Connection;

    class Poller{
        public:
        Poller(uint64_t poll_size=DEFAULT_EVENTS_LISTENED);
        ~Poller();
        NON_COPYABLE(Poller);
        void AddConnection(Connection* conn);
        std::vector<Connection*> Poll(int timeout=-1);
        uint64_t GetPollSize() const noexcept;

        private:
        int poll_fd_;
        uint64_t poll_size_;
        #ifdef OS_LINUX
        epoll_event* poll_events_{nullptr};
        #endif
    };
}
#endif




