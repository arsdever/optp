/*
 * =====================================================================================
 *
 *		Filename:	network_interfaces.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp_export.h>

namespace optp
{
	class OPTP_EXPORT network_interfaces
	{
	public:
		network_interfaces();
		bool is_local(std::string const& address) const;
		std::vector<std::string> local_addresses_string() const;
		std::vector<asio::ip::address> local_addresses() const;

		static network_interfaces& global();

	private:
		std::vector<asio::ip::address> m_localInterfaces;
	};
}