/*
 * =====================================================================================
 *
 *		Filename:	interface/opeartion.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <interface/typedefs.h>
#include <interface/serializable.h>
#include <interface/deserializable.h>
#include <string>

namespace optp
{
	class uuid;

	namespace interfaces
	{
		class operation : public serializable, public deserializable
		{
		public:
			virtual ~operation() = default;
			virtual std::string uuid() const = 0;
		};
	}
}
