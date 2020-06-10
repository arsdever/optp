/*
 * =====================================================================================
 *
 *		Filename:	optp/serializable.h
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
		class serializable
		{
		public:
			virtual std::string serialize() const = 0;
		};
	}
}