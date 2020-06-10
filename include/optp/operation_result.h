/*
 * =====================================================================================
 *
 *		Filename:	optp/opeartion.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp/typedefs.h>
#include <optp/serializable.h>
#include <optp/deserializable.h>
#include <string>

namespace optp
{
	namespace interfaces
	{
		class operation_result : public serializable, public deserializable
		{
		public:
			virtual ~operation_result() = default;
			virtual std::string nodeUuid() const = 0;
			virtual std::string uuid() const = 0;
		};
	}
}