/*
 * =====================================================================================
 *
 *       Filename:  object.cpp
 *         Author:  Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "uuid.h"
#include "uuid_provider.h"

namespace optp
{
	object::object()
		: __uuid(std::move(uuid_provider::global().provide))
	{

	}

	uuid object::uuid() const
	{
		return __uuid;
	}

	object object::clone() const
	{
		object cloned;
		return std::move(cloned);
	}
}
