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
            virtual operation_wptr handle(operation_wptr wop) = 0;
			virtual node_def_wptr handlerNodeDef() const = 0;
			virtual void setHandlerNodeDef(node_def_wptr handler_node_def) = 0;
        };
    }
}