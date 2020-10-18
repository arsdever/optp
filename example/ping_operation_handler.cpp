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
#include <node_def.h>

namespace optp
{
	namespace test
	{
		interfaces::operation_result_shptr ping_operation_handler::operator()(interfaces::operation_wptr wop)
		{
			const std::shared_ptr<ping_operation> ping = std::static_pointer_cast<ping_operation>(wop.lock());
			const node_def_shptr node_shptr = std::dynamic_pointer_cast<node_def>(handlerNodeDef().lock());
			interfaces::operation_result_shptr result = std::make_shared<ping_operation_result>(node_shptr->uuid(), ping->uuid());
			ping->setResult(result);
			return result;
		}
	}
}