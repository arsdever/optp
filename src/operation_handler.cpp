/*
 * =====================================================================================
 *
 *		Filename:	operation_handler.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */
#include "operation_handler.h"

namespace optp
{
	void operation_handler::setHandlerNode(interfaces::node_wptr handler_node)
	{
		m_handlerNode = std::move(handler_node);
	}

	interfaces::node_wptr operation_handler::handlerNode() const
	{
		return m_handlerNode;
	}
}