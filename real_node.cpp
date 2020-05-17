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

namespace optp
{
	real_node::real_node()
		: m_uuid(std::move(uuid_provider().provideRandomString()))
		, m_ipAddress(getLocalIpAddress())
	{
	}

	std::string real_node::address() const
	{
		return m_ipAddress;
	}
	
	interfaces::operation_ref real_node::execute(interfaces::operation_ref operation)
	{
		for(interfaces::node_wptr wnode : m_remoteNodes)
		{
			if(const interfaces::node_shptr shnode = wnode.lock())
				shnode->execute(operation);
		}

		return handle(operation);
	}

	interfaces::operation_ref real_node::handle(interfaces::operation_ref operation)
	{
		return operation;
	}

	void real_node::registerRemoteNode(interfaces::node_wptr remote_node)
	{
		m_remoteNodes.push_back(remote_node);
	}
	
	std::string real_node::uuid() const
	{
		return m_uuid;
	}
	
	std::string real_node::getLocalIpAddress()
	{
		return "127.0.0.1";
	}
}
