#include "disconnect_op.h"

namespace optp
{
	namespace operations
	{
		interfaces::operation_wptr disconnect_operation::disconnect_operation_handler::operator() (interfaces::operation_wptr operation)
		{
			/*if (std::shared_ptr<node_uuid_getter_operation> ptr = std::dynamic_pointer_cast<node_uuid_getter_operation>(operation.lock()))
			{
				std::string uuid = "";

				if (std::shared_ptr<node_def> defptr = std::dynamic_pointer_cast<node_def>(handlerNodeDef().lock()))
					uuid = defptr->uuid();

				operation_result_shptr result = std::make_shared<operation_result>(uuid, ptr->uuid());
				result->setResultData(nlohmann::json{ "uuid", uuid });
				ptr->setResult(result);
			}

			*/return operation;
		}
	}
}