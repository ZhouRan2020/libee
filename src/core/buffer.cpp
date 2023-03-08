#include "core/buffer.h"
#include <iterator>
#include <optional>
#include <string>
#include <vector>

namespace libee{
    Buffer::Buffer(size_t ini){
        buf_.reserve(ini);
    }

    void Buffer::Append(const unsigned char* new_char_data, size_t data_size){
        buf_.insert(buf_.end(),new_char_data,new_char_data+data_size);
    }    
    void Buffer::Append(const std::string& new_str_data){
        Append(reinterpret_cast<const unsigned char*>(new_str_data.c_str()),new_str_data.size());
    }
    void Buffer::Append(std::vector<unsigned char>&& other_buffer){
        buf_.insert(buf_.end(),std::make_move_iterator(other_buffer.begin()),std::make_move_iterator(other_buffer.end()));
    }

    void Buffer::Insert(const unsigned char* new_char_data,size_t data_size){
        buf_.insert(buf_.begin(),new_char_data,new_char_data+data_size);
    }    
    void Buffer::Insert(const std::string& new_str_data){
        Insert(reinterpret_cast<const unsigned char*>(new_str_data.c_str()),new_str_data.size());
    }

    std::optional<std::string> Buffer::FindAndPopTill(const std::string &target){
        std::optional<std::string> res = std::nullopt;
        auto curr_content = ToStringView();
        auto pos=curr_content.find(target);
        if(pos!=std::string::npos){
            res=curr_content.substr(0,pos+target.size());
            buf_.erase(buf_.begin(),buf_.begin()+pos+target.size());
        
        }
        return res;
    }


}