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
#include <string>

namespace optp
{
	namespace interfaces
	{
		class node : public std::enable_shared_from_this<node>
		{
		public:
			virtual ~node() = default;

			virtual std::string address() const = 0; // TODO: Should be changed to address type
			virtual interfaces::operation_shptr execute(interfaces::operation_shptr operation) = 0;
			virtual interfaces::operation_shptr handle(interfaces::operation_shptr operation) = 0;
			virtual void setProtocol(optp_wptr protocol) = 0;
			virtual node_def_wptr getDefinition() const = 0;
		};
	}
}