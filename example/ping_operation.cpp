/*
 * =====================================================================================
 *
 *		Filename:	example/ping_operation.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "ping_operation.h"
#include <sstream>

namespace optp
{
	namespace test
	{
		ping_operation::ping_operation()
		{
			setType((int)operation_types::PingOperation);
		}

		void ping_operation::setResult(interfaces::operation_result_shptr result) 
		{
			operation::setResult(result);
		}
	}
}