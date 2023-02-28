#ifndef SRC_INCLUDE_CORE_NET_ADDRESS_H
#define SRC_INCLUDE_CORE_NET_ADDRESS_H

#include <arpa/inet.h>

#include <cstring>
#include <iostream>
#include <string>

namespace libee{
	
	enum class Protocol {IPv4, IPv6};

	class NetAddress{
		public:
			NetAddress() noexcept;
			NetAddress(const char* ip, uint16_t port, Protocol protocol = Protocol::IPv4);
			~NetAddress() =default;

			Protocol GetProtocol() const noexcept;
			sockaddr* YieldAddr();
			socklen_t* YieldAddr();
			std::string GetIp() const noexcept;
			uint16_t GetPort() const noexcept;
			std::string ToString() const noexcept;
		private:
			Protocol protocol_{Protocol::IPv4};
			mutable sockaddr_storage addr_{};
			socklen_t addr_len_;
	};

	std::ostream& operator<< (std::ostream& os, const NetAddress& address);
}

#endif

