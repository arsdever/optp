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
#include <optp/object.h>

namespace optp
{
	class uuid;

	namespace interfaces
	{
		class operation : std::enable_shared_from_this<operation>
		{
		public:
			virtual ~operation() = default;
			virtual int type() const = 0;
			virtual void setResult(operation_result_shptr result) = 0;
			virtual std::list<operation_result_wptr> getResults() const = 0;

		protected:
			virtual void setType(int operation_type) = 0;
		};
	}
}
