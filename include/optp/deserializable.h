/*
 * =====================================================================================
 *
 *		Filename:	optp/deserializable.h
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
		class deserializable
		{
		public:
			virtual void deserialize(std::string const& dataBuffer) = 0;
		};
	}
}