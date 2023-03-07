/*This is a header file implementing the Internet Address, which is composed of an IP address and a port*/

#ifndef SRC_INCLUDE_CORE_NET_ADDRESS_H
#define SRC_INCLUDE_CORE_NET_ADDRESS_H

#include <arpa/inet.h>

#include <cstring>
#include <iostream>
#include <string>

namespace libee{
	
	/*network protocol supported*/
	enum class Protocol {IPv4, IPv6};

	/*This Netaddress class encapsulates the unique identifier of a network host in the form of "IP Address + Port"
	 This class is compatible with both IPv4 and IPv6*/ 
	class NetAddress{
		public:
			NetAddress() noexcept{
				memset(&addr_,0,sizeof(addr_));
				addr_len_=sizeof(addr_);
			}

			NetAddress(const char* ip, uint16_t port, Protocol protocol = Protocol::IPv4);
			~NetAddress() =default;

			Protocol GetProtocol() const noexcept{return protocol_;} ;
			sockaddr* YieldAddr() {return reinterpret_cast<sockaddr*>(&addr_);}
			socklen_t* YieldAddrLen(){return &addr_len_;}
			std::string GetIP() const noexcept;
			uint16_t GetPort() const noexcept;
			std::string ToString() const noexcept{
				return GetIP()+"@"+std::to_string(GetPort());
			}
		private:
			Protocol protocol_{Protocol::IPv4};
			mutable sockaddr_storage addr_{};
			socklen_t addr_len_;
	};

	std::ostream& operator<< (std::ostream& os, const NetAddress& address);
}

#endif

