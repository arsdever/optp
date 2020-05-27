#include "network_interfaces.h"
#include <sockpp/inet_address.h>
#include <sockpp/inet6_address.h>
#include <string>
#include <vector>

#if defined (_WIN32)
#include <iphlpapi.h>
#elif defined (__linux__)
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#endif

static void collectLocalIpAddresses(std::vector<std::shared_ptr<sockpp::sock_address>>& address_list)
{
#if defined (_WIN32)
#pragma comment(lib, "IPHLPAPI.lib")
	// https://stackoverflow.com/questions/122208/get-the-ip-address-of-local-computer
	PIP_ADAPTER_ADDRESSES adapter_addresses(NULL);
	DWORD adapter_addresses_buffer_size = 16000;

	for (int attempts = 0; attempts != 3; ++attempts)
	{
		adapter_addresses = (IP_ADAPTER_ADDRESSES*)malloc(adapter_addresses_buffer_size);

		DWORD error = ::GetAdaptersAddresses(
			AF_UNSPEC,
			GAA_FLAG_SKIP_ANYCAST |
			GAA_FLAG_SKIP_MULTICAST |
			GAA_FLAG_SKIP_DNS_SERVER |
			GAA_FLAG_SKIP_FRIENDLY_NAME,
			NULL,
			adapter_addresses,
			&adapter_addresses_buffer_size);

		if (ERROR_SUCCESS == error)
			break;
		else if (ERROR_BUFFER_OVERFLOW == error)
		{
			free(adapter_addresses);
			adapter_addresses = NULL;
			continue;
		}
		else
		{
			free(adapter_addresses);
			adapter_addresses = NULL;
			return;
		}
	}

	for (PIP_ADAPTER_ADDRESSES adapter = adapter_addresses; adapter != NULL; adapter = adapter->Next)
	{
		if (IF_TYPE_SOFTWARE_LOOPBACK == adapter->IfType)
			continue;

		for (PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress; NULL != address; address = address->Next)
		{
			auto family = address->Address.lpSockaddr->sa_family;
			if (AF_INET == family)
			{
				SOCKADDR_IN* ipv4 = reinterpret_cast<SOCKADDR_IN*>(address->Address.lpSockaddr);
				address_list.push_back(std::make_shared<sockpp::inet_address>(*ipv4));
			}
			else if (AF_INET6 == family)
			{
				SOCKADDR_IN6* ipv6 = reinterpret_cast<SOCKADDR_IN6*>(address->Address.lpSockaddr);
				auto addr = std::make_shared<sockpp::inet6_address>(*ipv6);
				if (0 == addr->to_string().find("fe"))
				{
					char c = addr->to_string()[2];
					if (c == '8' || c == '9' || c == 'a' || c == 'b')
						address_list.push_back(addr);
				}
			}
		}
	}

	free(adapter_addresses);
	adapter_addresses = NULL;
#elif defined (__linux__)
	ifaddrs* interfaces = nullptr;
	getifaddrs(&interfaces);
	for (ifaddrs* ifa = interfaces; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (!ifa->ifa_addr)
			continue;

		if (ifa->ifa_addr->sa_family == AF_INET)
			address_list.push_back(std::make_shared<sockpp::inet_address>(*ifa->ifa_addr));

		else if (ifa->ifa_addr->sa_family == AF_INET6)
			address_list.push_back(std::make_shared<sockpp::inet6_address>(*ifa->ifa_addr));
	}
#endif
}

namespace optp
{
	network_interfaces::network_interfaces()
	{
		collectLocalIpAddresses(m_local_interfaces);
	}

	bool network_interfaces::is_local(sockpp::sock_address const& address) const
	{
		std::string addr_str = address.to_string().substr();
		std::string ip_str = addr_str.substr(0, addr_str.find_first_of(':'));
		return is_local(ip_str);
	}

	bool network_interfaces::is_local(std:: string const& address) const
	{
		return std::find_if(m_local_interfaces.begin(), m_local_interfaces.end(), [&address](auto addr) -> bool {
			std::string addr_str = addr->to_string();
			std::string ip_str = addr_str.substr(0, addr_str.find_first_of(':'));
			return address == ip_str;
			}) != m_local_interfaces.end();
	}

	network_interfaces& network_interfaces::global()
	{
		static network_interfaces instance;
		return instance;
	}
}