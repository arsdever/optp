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

			// interfaces::node started
			virtual std::string address() const = 0; // TODO: Should be changed to address type
			virtual interfaces::operation& execute(interfaces::operation& operation) = 0;
			virtual interfaces::operation& handle(interfaces::operation& operation) = 0;
			virtual std::string uuid() const = 0;
			// interfaces::node finished
		};
	}
}