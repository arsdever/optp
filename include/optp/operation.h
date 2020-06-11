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
	class uuid;

	namespace interfaces
	{
		class operation : public serializable, public deserializable
		{
		public:
			virtual ~operation() = default;
			virtual int type() const = 0;
			virtual std::string uuid() const = 0;
			virtual void setResult(operation_result_shptr result) = 0;
		};
	}
}
