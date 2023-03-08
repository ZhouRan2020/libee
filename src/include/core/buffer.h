/*This is a header file implementing the char buffer to push-in and pop-out bytes in order*/

#ifndef SRC_INCLUDE_CORE_BUFFER_H
#define SRC_INCLUDE_CORE_BUFFER_H
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/utils.h"

namespace libee{

    /*default ini underlying capacity of buffer*/
    static constexpr size_t INITIAL_BUFFER_CAPACITY=1024;

    /*This buffer abstracts an underlying dynmic char array that allows pushing in byte data from two ends
    not thread-safe*/
    class Buffer{
    public:
        Buffer(size_t initial_capacity=INITIAL_BUFFER_CAPACITY);
        ~Buffer() =default;
        Buffer(const Buffer& other) =default;
        Buffer& operator= (const Buffer& other) =default;
        NON_MOVEABLE(Buffer);

        void Append(const unsigned char* new_char_data, size_t data_size);
        void Append(const std::string& new_str_data);
        void Append(std::vector<unsigned char>&& other_buffer);

        void Insert(const unsigned char* new_char_data, size_t data_size);
        void Insert(const std::string& new_str_data);

        std::optional<std::string> FindAndPopTill(const std::string& target);
        size_t size(){return buf_.size();}
        size_t capacity(){return buf_.capacity();}
        const unsigned char* data(){return buf_.data();}

        std::string_view ToStringView() const noexcept{
            return {reinterpret_cast<const char*>(buf_.data()),buf_.size()};
        }
        void clear() noexcept {buf_.clear();}

    private:
        std::vector<unsigned char> buf_;

    };
    

}


#endif
