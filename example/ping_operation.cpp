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
			
		}

		int ping_operation::type() const 
		{
			return (int) operation_types::PingOperation;
		}

		std::string ping_operation::serialize() const 
		{
			return "ping";
		}

		void ping_operation::deserialize(std::string const& data) 
		{

		}

		void ping_operation::setResult(interfaces::operation_result_shptr result) 
		{

		}
	}
}