// Reference: https://stackoverflow.com/questions/2674314/get-local-ip-address-using-boost-asio

#include "network_interfaces.h"

namespace optp
{
	namespace
	{
		asio::ip::address_v6 sinaddr_to_asio(sockaddr_in6* addr) {
			asio::ip::address_v6::bytes_type buf;
			memcpy(buf.data(), addr->sin6_addr.s6_addr, sizeof(addr->sin6_addr));
			return asio::ip::make_address_v6(buf, addr->sin6_scope_id);
		}

#if defined(_WIN32)
#undef UNICODE
#include <winsock2.h>
		// Headers that need to be included after winsock2.h:
#include <iphlpapi.h>
#include <ws2ipdef.h>

		typedef IP_ADAPTER_UNICAST_ADDRESS_LH Addr;
		typedef IP_ADAPTER_ADDRESSES* AddrList;

		std::vector<asio::ip::address> get_local_interfaces() {
			// It's a windows machine, we assume it has 512KB free memory
			DWORD outBufLen = 1 << 19;
			AddrList ifaddrs = (AddrList) new char[outBufLen];

			std::vector<asio::ip::address> res;

			ULONG err = GetAdaptersAddresses(AF_UNSPEC,
				GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_DNS_SERVER, NULL, ifaddrs,
				&outBufLen);

			if (err == NO_ERROR) {
				for (AddrList addr = ifaddrs; addr != 0; addr = addr->Next) {
					if (addr->OperStatus != IfOperStatusUp) continue;
					// if (addr->NoMulticast) continue;

					// Find the first IPv4 address
					if (addr->Ipv4Enabled) {
						for (Addr* uaddr = addr->FirstUnicastAddress; uaddr != 0; uaddr = uaddr->Next) {
							if (uaddr->Address.lpSockaddr->sa_family != AF_INET) continue;
							res.push_back(asio::ip::make_address_v4(ntohl(reinterpret_cast<sockaddr_in*>(uaddr->Address.lpSockaddr)->sin_addr.s_addr)));
						}
					}

					if (addr->Ipv6Enabled) {
						for (Addr* uaddr = addr->FirstUnicastAddress; uaddr != 0; uaddr = uaddr->Next) {
							if (uaddr->Address.lpSockaddr->sa_family != AF_INET6) continue;
							res.push_back(sinaddr_to_asio(reinterpret_cast<sockaddr_in6*>(uaddr->Address.lpSockaddr)));
						}
					}
				}
			}
			else {

			}
			delete[]((char*)ifaddrs);
			return res;
		}
#elif defined(__APPLE__) || defined(__linux__)
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/types.h>

		std::vector<asio::ip::address> get_local_interfaces() {
			std::vector<asio::ip::address> res;
			ifaddrs* ifs;
			if (getifaddrs(&ifs)) {
				return res;
			}
			for (auto addr = ifs; addr != nullptr; addr = addr->ifa_next) {
				// No address? Skip.
				if (addr->ifa_addr == nullptr) continue;

				// Interface isn't active? Skip.
				if (!(addr->ifa_flags & IFF_UP)) continue;

				if (addr->ifa_addr->sa_family == AF_INET) {
					res.push_back(asio::ip::make_address_v4(ntohl(
						reinterpret_cast<sockaddr_in*>(addr->ifa_addr)->sin_addr.s_addr)));
				}
				else if (addr->ifa_addr->sa_family == AF_INET6) {
					res.push_back(sinaddr_to_asio(reinterpret_cast<sockaddr_in6*>(addr->ifa_addr)));
				}
				else continue;
			}
			freeifaddrs(ifs);
			return res;
		}
#else
#error "Unsupported platform"
#endif
	}

	network_interfaces::network_interfaces()
		: m_localInterfaces{std::move(get_local_interfaces())}
	{}

	//bool is_local(sockpp::sock_address const& address) const;
	bool network_interfaces::is_local(std::string const& address) const
	{
		return std::find_if(m_localInterfaces.cbegin(), m_localInterfaces.cend(), [&address](asio::ip::address const& interface_address) {
			return asio::ip::make_address(address) == interface_address; }) != m_localInterfaces.cend();
	}

	std::vector<std::string> network_interfaces::local_addresses_string() const
	{
		std::vector<std::string> result;
		for (asio::ip::address address : m_localInterfaces)
			result.push_back(address.to_string());

		return std::move(result);
	}

	std::vector<asio::ip::address> network_interfaces::local_addresses() const
	{
		return m_localInterfaces;
	}

	network_interfaces& network_interfaces::global()
	{
		static network_interfaces instance;
		return instance;
	}
}