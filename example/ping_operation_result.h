/*
 * =====================================================================================
 *
 *		Filename:	example/ping_operation_result.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */
#pragma once

#include <operation_result.h>

namespace optp
{
	namespace test
	{
		class ping_operation_result : public operation_result
		{
		public:
			inline ping_operation_result(std::string const& nodeUuid)
				: operation_result(nodeUuid)
			{}
			inline std::string message() const { return "received"; }
		};
	}
}
