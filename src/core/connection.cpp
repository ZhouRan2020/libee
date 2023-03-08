#include "core/connection.h"
#include "core/buffer.h"
#include "core/socket.h"

#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstdio>
#include <memory>
#include <sys/socket.h>

#include <cstring>
#include <sys/types.h>
#include <utility>

namespace libee{
    Connection::Connection(std::unique_ptr<Socket> socket)
        :socket_(std::move(socket)),read_buffer_(std::make_unique<Buffer>()),write_buffer_(std::make_unique<Buffer>())
        {}
    std::pair<ssize_t,bool> Connection::Recv(){
        int  from_fd=GetFd();
        ssize_t read=0;
        unsigned char buf[TEMP_BUF_SIZE+1];
        memset(buf,0,sizeof(buf));
        while(1){
            ssize_t curr_read=recv(from_fd, buf, TEMP_BUF_SIZE, 0);
            if(curr_read>0){
                read+=curr_read;
                WriteToReadBuffer(buf,curr_read);
                memset(buf,0,sizeof(buf));
            }
            else if(curr_read==0){
                return {read,true};
            }
            else if (curr_read==-1&&errno==EINTR) {
                continue;
            }
            else if(curr_read==-1&&(errno==EAGAIN||errno==EWOULDBLOCK)){
                break;
            }
            else{
                perror("handleconnection: recv() error");
                return {read,true};
            
            }
        }
        return {read,false};
    }

    void Connection::Send(){
        ssize_t curr_write =0 ;
        ssize_t write;
        const ssize_t to_write = GetWriteBufferSize();
        const unsigned char* buf = write_buffer_->data();
        while(curr_write<to_write){
            write=send(GetFd(),buf+curr_write,to_write-curr_write,0);
            if(write<=0){
                if(errno!=EINTR&&errno!=EAGAIN&&errno!=EWOULDBLOCK){
                    perror("error in con::send");
                    ClearWriteBuffer();
                    return;
                }
                write=0;
            }
            curr_write+=write;
        }
        ClearWriteBuffer();
    }

}