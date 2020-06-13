/*
 * =====================================================================================
 *
 *		Filename:	operation_handler.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */
#pragma once

#include <optp/operation_handler.h>

namespace optp
{
	class operation_handler : public interfaces::operation_handler
	{
	public:
		void setHandlerNode(interfaces::node_wptr handler_node) override;
		interfaces::node_wptr handlerNode() const override;

	private:
		interfaces::node_wptr m_handlerNode;
	};
}