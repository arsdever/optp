/*
 * =====================================================================================
 *
 *		Filename:	optp/node_def.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp/typedefs.h>
#include <optp/serializable.h>
#include <optp/deserializable.h>

namespace optp
{
	namespace interfaces
	{
		class node_def : public serializable, public deserializable
		{
		public:
			virtual ~node_def() = default;

			virtual std::string address() const = 0; // TODO: Should be changed to address type
			virtual std::string uuid() const = 0;
		};
	}
}
