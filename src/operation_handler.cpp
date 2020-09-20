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
	interfaces::operation_wptr operation_handler::handle(interfaces::operation_wptr op)
	{
		return (*this)(op);
	}

	void operation_handler::setHandlerNodeDef(interfaces::node_def_wptr handler_node_def)
	{
		m_handlerNodeDef = std::move(handler_node_def);
	}

	interfaces::node_def_wptr operation_handler::handlerNodeDef() const
	{
		return m_handlerNodeDef;
	}
}