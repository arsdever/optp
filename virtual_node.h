/*
 * =====================================================================================
 *
 *		Filename:	virtual_node.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include <interface/node.h>

namespace optp
{
	class virtual_node : public interfaces::node
	{
	public:
		virtual_node(interfaces::node_wptr impl_node);

		// interface::node started
		std::string address() const override;
		interfaces::operation_ref execute(interfaces::operation_ref operation) override;
		interfaces::operation_ref handle(interfaces::operation_ref operation) override;
		std::string uuid() const override;
		// interface::node finished

	private:
		interfaces::node_wptr m_parentNode;
		std::string m_uuid;
	};
}
