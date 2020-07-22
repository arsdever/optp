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
#include <istream>

namespace optp
{
	namespace interfaces
	{
		class deserializable
		{
		public:
			virtual std::istream& deserialize(std::istream& stm) = 0;
		};
	}
}