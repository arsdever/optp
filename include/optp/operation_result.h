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
//#include <string>

namespace optp
{
	namespace interfaces
	{
		class operation_result : std::enable_shared_from_this<operation_result>
		{
		public:
			virtual ~operation_result() = default;
			virtual std::string operationUuid() const = 0;
			virtual std::string nodeUuid() const = 0;
		};
	}
}