/*
 * =====================================================================================
 *
 *		Filename:	optp/opeartion_handler.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */
#pragma once

#include <optp/typedefs.h>

namespace optp
{
    namespace interfaces
    {
        class operation_handler
        {
        public:
            virtual ~operation_handler() = default;
            virtual void handle(operation_wptr wop) = 0;
			virtual node_wptr handlerNode() const = 0;
			virtual void setHandlerNode(interfaces::node_wptr handler_node) = 0;
        };
    }
}