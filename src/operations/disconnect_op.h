#pragma once

#include <operation.h>
#include <operation_handler.h>
#include <operation_result.h>
#include <node_def.h>
#include <operations/operation_types.h>

namespace optp
{
	namespace operations
	{
		class disconnect_operation : public operation
		{
		public:
			disconnect_operation() : operation(type_id()) {}

			typedef class disconnect_operation_handler : public operation_handler
			{
			public:
				interfaces::operation_result_shptr operator()(interfaces::operation_wptr operation);
			} handler_type;

			inline static int type_id() { return (int)operation_types::NodeUuidGetter; }
		};
	}
}