/*
 * =====================================================================================
 *
 *		Filename:	real_node.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "real_node.h"
#include "node_def.h"

#include <optp/optp.h>
#include <optp/operation.h>
#include <optp/operation_handler.h>
#include "operation_result.h"

namespace optp
{
	real_node::real_node(interfaces::optp_wptr protocol)
		: m_protocol(protocol)
		, m_definition(std::move(std::make_shared<node_def>("127.0.0.1")))
	{
	}

	std::string real_node::address() const
	{
		return "127.0.0.1"; // TODO: Fix the implementation
	}
	
	interfaces::operation_shptr real_node::execute(interfaces::operation_shptr operation)
	{
		if (interfaces::optp_shptr protocol = m_protocol.lock())
		{
			return protocol->execute(operation);
		}

		operation->setResult(handle(operation));
		return operation;
	}

	interfaces::operation_result_shptr real_node::handle(interfaces::operation_shptr operation)
	{
		auto handler_iterator = m_handlersMap.find(operation->type());
		if (handler_iterator != m_handlersMap.end() && handler_iterator->second != nullptr)
		{
			handler_iterator->second->handle(operation);
		}
		
		return std::make_shared<operation_result>(
			std::dynamic_pointer_cast<interfaces::object>(getDefinition().lock())->uuid(),
			std::dynamic_pointer_cast<interfaces::object>(operation)->uuid());
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
