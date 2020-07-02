/*
 * =====================================================================================
 *
 *		Filename:	example/ping_operation_handler.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */
#pragma once

#include <operation_handler.h>

namespace optp
{
	namespace test
	{
		class ping_operation_handler : public operation_handler
		{
		public:
			void handle(interfaces::operation_wptr wop) override;
		};
	}
}