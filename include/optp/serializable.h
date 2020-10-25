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

namespace optp
{
	namespace interfaces
	{
		class serializable
		{
		public:
			virtual std::ostream& serialize(std::ostream& stm) const = 0;
		};
	}
}