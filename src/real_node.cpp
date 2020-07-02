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
#include "node_def.h"

#include <optp/optp.h>
#include <optp/operation.h>
#include <optp/operation_handler.h>

namespace optp
{
	real_node::real_node(interfaces::optp_wptr protocol)
		: m_uuid(std::move(uuid_provider().provideRandomString()))
		, m_protocol(protocol)
		, m_definition(std::move(std::make_shared<node_def>("127.0.0.1")))
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

		return handle(operation);
	}

	interfaces::operation_shptr real_node::handle(interfaces::operation_shptr operation)
	{
		auto handler_iterator = m_handlersMap.find(operation->type());
		if (handler_iterator != m_handlersMap.end() && handler_iterator->second != nullptr)
		{
			handler_iterator->second->handle(operation);
		}
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

	interfaces::node_def_wptr real_node::getDefinition() const
	{
		return m_definition;
	}
	
	void real_node::registerOperationHandler(int operation_type, interfaces::operation_handler_shptr handler)
	{
		m_handlersMap[operation_type] = handler;
	}
}
