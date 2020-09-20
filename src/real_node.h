/*
 * =====================================================================================
 *
 *		Filename:	real_node.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include <optp/node.h>
#include <list>
#include <chrono>

namespace optp
{
	class real_node : public interfaces::node
	{
	public:
		real_node(interfaces::optp_wptr protocol);

		std::string address() const override;
		interfaces::operation_shptr execute(interfaces::operation_shptr operation) override;
		interfaces::operation_shptr handle(interfaces::operation_shptr operation) override;
		interfaces::node_def_wptr getDefinition() const override;

		void registerOperationHandler(int operation_type, interfaces::operation_handler_shptr handler);

	private:
		interfaces::optp_wptr m_protocol;
		std::list<interfaces::node_shptr> m_remoteNodes;
		interfaces::node_def_shptr m_definition;
	};
}
