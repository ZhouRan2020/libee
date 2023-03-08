#ifndef SRC_INCLUDE_CORE_CONNECTION_H
#define SRC_INCLUDE_CORE_CONNECTION_H

#include <functional> 
#include <memory>
#include <optional>
#include <string>
#include <sys/types.h>
#include <utility> 
#include <vector>

#include "core/buffer.h"
#include "core/socket.h"
#include "core/utils.h"

namespace libee{
    static constexpr int TEMP_BUF_SIZE = 2048;
    class Looper;
    class Connection{
        public:
            Connection(std::unique_ptr<Socket> socket);
            ~Connection() = default;
            NON_COPYABLE(Connection);

            int GetFd() const noexcept {return socket_->GetFd();}
            Socket* GetSocket() noexcept {return socket_.get();}

            /* for poller */
            void SetEvents(uint32_t events){events_=events;}
            uint32_t GetEvents() const noexcept{return events_;}
            void SetRevents(uint32_t revents){revents_=revents;}
            uint32_t GetRevents() const noexcept{return revents_;}

            void SetCallback(const std::function<void(Connection*)> &callback){
                callback_=[callback,this]{return callback(this);};
            }
            std::function<void()> GetCallback() noexcept{return callback_;}

            /*for buffer*/
            std::optional<std::string> FindAndPopTill(const std::string& target){return read_buffer_->FindAndPopTill(target);}
            size_t GetReadBufferSize() const noexcept{return read_buffer_->size();}
            size_t GetWriteBufferSize() const noexcept{return write_buffer_->size();}
            void WriteToReadBuffer(const unsigned char* buf, size_t size){read_buffer_->Append(buf,size);}
            void WriteToWriteBuffer(const unsigned char* buf, size_t size){write_buffer_->Append(buf,size);}
            void WriteToReadBuffer(const std::string& str){read_buffer_->Append(str);}
            void WriteToWriteBuffer(const std::string& str){write_buffer_->Append(str);}
            void WriteToWriteBuffer(std::vector<unsigned char>&& other_buf){write_buffer_->Append(std::move(other_buf));}
            const unsigned char* Read() const noexcept{return read_buffer_->data();}
            std::string ReadAsString() const noexcept{
                auto str_view=read_buffer_->ToStringView();
                return {str_view.begin() , str_view.end()};
            }

            /**/
            std::pair<ssize_t,bool> Recv();
            void Send();
            void ClearReadBuffer() noexcept{read_buffer_->clear();}
            void ClearWriteBuffer() noexcept{write_buffer_->clear();}

            void SetLooper(Looper* looper) noexcept{owner_looper_=looper;}
            Looper* GetLooper() noexcept{return owner_looper_;}

        private:
            Looper* owner_looper_{nullptr};
            std::unique_ptr<Socket> socket_;
            std::unique_ptr<Buffer> read_buffer_;
            std::unique_ptr<Buffer> write_buffer_;
            uint32_t events_{0};
            uint32_t revents_{0};
            std::function<void()> callback_{nullptr};
    };
}
#endif