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
#include <typedefs.h>

namespace optp
{
	class operation_handler : public interfaces::operation_handler
	{
	public:
		interfaces::operation_wptr handle(interfaces::operation_wptr op) override;
		void setHandlerNodeDef(interfaces::node_def_wptr handler_node_def) override;
		interfaces::node_def_wptr handlerNodeDef() const override;

		virtual interfaces::operation_wptr operator()(interfaces::operation_wptr operation) = 0; 

	private:
		interfaces::node_def_wptr m_handlerNodeDef;
	};
}