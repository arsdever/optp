/*
 * =====================================================================================
 *
 *		Filename:	real_node.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "real_node.h"
#include "uuid_provider.h"
#include <optp/optp.h>
#include <optp/operation.h>

namespace optp
{
	real_node::real_node(interfaces::optp_wptr protocol)
		: m_uuid(std::move(uuid_provider().provideRandomString()))
		, m_protocol(protocol)
	{
	}

	std::string real_node::address() const
	{
		return "127.0.0.1";
	}
	
	interfaces::operation_shptr real_node::execute(interfaces::operation_shptr operation)
	{
		if (interfaces::optp_shptr protocol = m_protocol.lock())
		{
			return protocol->handle(operation);
		}
		return operation;
	}

	interfaces::operation_shptr real_node::handle(interfaces::operation_shptr operation)
	{
		return operation;
	}
	
	std::string real_node::uuid() const
	{
		return m_uuid;
	}

	void real_node::setProtocol(interfaces::optp_wptr protocol)
	{
		m_protocol = protocol;
	}
}
