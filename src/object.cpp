/*
 * =====================================================================================
 *
 *       Filename:  object.cpp
 *         Author:  Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "object.h"
#include "uuid_provider.h"

namespace optp
{
	object::object(object_metatype metatype)
		: __uuid(std::move(uuid_provider().provideRandomString(64)))
		, __metatype(metatype)
	{

	}

	std::string object::uuid() const
	{
		return __uuid;
	}

	interfaces::object_shptr object::clone() const
	{
		return nullptr;
	}

	int object::metatype() const
	{
		return (int)__metatype;
	}

	std::ostream& object::serialize(std::ostream& stm) const
	{
		stm << uuid() << metatype();
		return stm;
	}

	std::istream& object::deserialize(std::istream& stm)
	{
		char uuid[64];
		stm.read(uuid, 64);
		__uuid = std::string(uuid, 64);
		int mt;
		stm >> mt;
		__metatype = (object_metatype)mt;
		return stm;
	}
}
