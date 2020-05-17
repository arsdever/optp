/*
 * =====================================================================================
 *
 *		Filename:	virtual_node.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "virtual_node.h"
#include "uuid_provider.h"

namespace optp
{
	virtual_node::virtual_node(interfaces::node_wptr impl_node)
		: m_uuid(std::move(uuid_provider().provideRandomString()))
		, m_parentNode(impl_node)
	{}

	std::string virtual_node::address() const
	{
		if (const interfaces::node_shptr shparent = m_parentNode.lock())
			return shparent->address();

		return "";
	}

	interfaces::operation_ref virtual_node::execute(interfaces::operation_ref operation)
	{
		if (const interfaces::node_shptr shparent = m_parentNode.lock())
			return shparent->handle(operation);

		return operation;
	}

	interfaces::operation_ref virtual_node::handle(interfaces::operation_ref operation)
	{
		if (const interfaces::node_shptr shparent = m_parentNode.lock())
			return shparent->handle(operation);

		return operation;
	}

	std::string virtual_node::uuid() const
	{
		return m_uuid;
	}
}