#include "core/net_address.h"

namespace libee{
	NetAddress::NetAddress(const char* ip, uint16_t port, Protocol protocol)
		:protocol_{protocol}
	{
		memset(&addr_,0,sizeof(addr_));
		addr_len_=sizeof(addr_);
		if(protocol_ == Protocol::IPv4){
			auto addr_ipv4 = reinterpret_cast<sockaddr_in*>(&addr_);
			addr_ipv4->sin_family=AF_INET;
			inet_pton(AF_INET,ip,&addr_ipv4->sin_addr.s_addr);
			addr_ipv4->sin_port=htons(port);
		}
		else{
			auto addr_ipv6 = reinterpret_cast<sockaddr_in*>(&addr_);
			addr_ipv6->sin_family=AF_INET6;
			inet_pton(AF_INET6,ip,&addr_ipv6->sin_addr.s_addr);
			addr_ipv6->sin_port=htons(port);
		}
	}
	std::string NetAddress::GetIP() const noexcept {
		char ip_address[INET6_ADDRSTRLEN];
		if(protocol_==Protocol::IPv4){
			auto addr_ipv4=reinterpret_cast<sockaddr_in*>(&addr_);
			inet_ntop(AF_INET,&addr_ipv4->sin_addr,ip_address,INET_ADDRSTRLEN);
		}
		else{
			auto addr_ipv6=reinterpret_cast<sockaddr_in6*>(&addr_);
			inet_ntop(AF_INET6,&addr_ipv6->sin6_addr,ip_address,INET6_ADDRSTRLEN);
		}
		return ip_address;
	}

	uint16_t NetAddress::GetPort() const noexcept{
		uint16_t port;
		if(protocol_==Protocol::IPv4){
			auto addr_ipv4=reinterpret_cast<sockaddr_in*>(&addr_);
			port=ntohs(addr_ipv4->sin_port);
		}
		else{
			auto addr_ipv6=reinterpret_cast<sockaddr_in6*>(&addr_);
			port=ntohs(addr_ipv6->sin6_port);
		}
		return port;
	}

	std::ostream& operator<< (std::ostream& os, const NetAddress& address){
		os<<address.ToString();
		return os;
	}
}