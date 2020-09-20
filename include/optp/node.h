/*
 * =====================================================================================
 *
 *		Filename:	optp/node.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include <optp/typedefs.h>
#include <operation_handler.h>

namespace optp
{
	namespace interfaces
	{
		class node : public std::enable_shared_from_this<node>
		{
		public:
			virtual ~node() = default;

			virtual std::string address() const = 0;
			virtual operation_shptr execute(interfaces::operation_shptr operation) = 0;
			virtual operation_shptr handle(interfaces::operation_shptr operation) = 0;
			virtual node_def_wptr getDefinition() const = 0;

			template <typename T>
			void registerAnOperationType()
			{
				interfaces::operation_handler_shptr handler = std::make_shared<typename T::handler_type>();
				handler->setHandlerNodeDef(this->getDefinition());
				m_handlersMap.insert(std::make_pair(T::type_id(), handler));
			}

		protected:
			std::unordered_map<int, interfaces::operation_handler_shptr> m_handlersMap;
		};
	}
}