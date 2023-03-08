#include "core/poller.h"

#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <vector>

#include "core/connection.h"

namespace libee{
    #ifdef OS_LINUX
    Poller::Poller(uint64_t poll_size):poll_size_(poll_size){
        poll_fd_=epoll_create1(0);
        if(poll_fd_==-1){
            perror("poll : epoll_create1() error ");
            exit(EXIT_FAILURE);
        }
        poll_events_=new epoll_event[poll_size];
        memset(poll_events_,0,poll_size_*sizeof(epoll_event));
    }
    #endif

    Poller::~Poller(){
        if(poll_fd_!=-1){
            close(poll_fd_);
            delete[] poll_events_;
            poll_fd_ = -1;
        }
    }

    #ifdef OS_LINUX
    void Poller::AddConnection(Connection* conn){
        assert(conn->GetFd()!=-1 && "can not Addconnection with an invalid fd");
        epoll_event event;
        memset(&event,0 , sizeof(event));
        event.data.ptr=conn;
        event.events=conn->GetEvents();
        int ret_val = epoll_ctl(poll_fd_,POLL_ADD,conn->GetFd(),&event);
        if(ret_val==-1){
            perror("Poller: epoll_ctl add error");
            exit(EXIT_FAILURE);
        }
    }
    #endif

    #ifdef OS_LINUX
        std::vector<Connection*> Poller::Poll(int timeout){
            std::vector<Connection*> events_happen;
            int ready=epoll_wait(poll_fd_,poll_events_,poll_size_,timeout);
            if(ready==-1){
                perror("poller: poll() error");
                exit(EXIT_FAILURE);
            }
            for(int i=0;i<ready;i++){
                Connection* ready_conn=reinterpret_cast<Connection*>(poll_events_[i].data.ptr);
                ready_conn->SetEvents(poll_events_[i].events);
                events_happen.emplace_back(ready_conn);
            }
            return events_happen;
        }
    #endif
}