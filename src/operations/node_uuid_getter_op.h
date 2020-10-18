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
		class node_uuid_getter_operation : public operation
		{
		public:
			node_uuid_getter_operation() : operation(type_id()) {}

			typedef class node_uuid_getter_operation_handler : public operation_handler
			{
			public:
				interfaces::operation_result_shptr operator()(interfaces::operation_wptr operation)
				{
					std::shared_ptr<node_uuid_getter_operation> ptr = std::dynamic_pointer_cast<node_uuid_getter_operation>(operation.lock());
					assert(!ptr);
					std::string uuid = "";
					if (std::shared_ptr<node_def> defptr = std::dynamic_pointer_cast<node_def>(handlerNodeDef().lock()))
						uuid = defptr->uuid();

					operation_result_shptr result = std::make_shared<operation_result>(uuid, ptr->uuid());
					result->setResultData(nlohmann::json{ "uuid", uuid });
					return result;
				}
			} handler_type;

			inline static int type_id() { return (int)operation_types::NodeUuidGetter; }
		};
	}
}