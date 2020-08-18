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
#include <sockpp/sock_address.h>

#include <vector>

namespace optp
{
	class OPTP_EXPORT network_interfaces
	{
	public:
		network_interfaces();
		bool is_local(sockpp::sock_address const& address) const;
		bool is_local(std::string const& address) const;
		std::vector<std::string> localAddresses() const;

		static network_interfaces& global();

	private:
		std::vector<std::shared_ptr<sockpp::sock_address>> m_local_interfaces;
	};
}