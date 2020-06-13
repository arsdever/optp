/*
 * =====================================================================================
 *
 *		Filename:	example/ping_operation_handler.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "ping_operation_handler.h"
#include "ping_operation.h"
#include "ping_operation_result.h"
#include <optp/node.h>

namespace optp
{
	namespace test
	{
		void ping_operation_handler::handle(interfaces::operation_wptr wop)
		{
			if (const std::shared_ptr<ping_operation> ping = std::static_pointer_cast<ping_operation>(wop.lock()))
			{
				if (const auto node_shptr = handlerNode().lock())
				{
					interfaces::operation_result_shptr result = std::make_shared<ping_operation_result>(node_shptr->uuid());
					ping->setResult(result);
				}
			}
		}
	}
}