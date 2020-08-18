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
	std::string normalize(int data, int size = 5)
	{
		std::string txt = std::to_string(data);
		while (txt.size() < size)
			txt = '0' + txt;

		return txt;
	}

	object::object()
		: __uuid(uuid_provider().provideRandomString(64))
	{}

	std::ostream& object::serialize(std::ostream& stm) const
	{
		stm << normalize(metatype()) << uuid();
		return stm;
	}

	std::istream& object::deserialize(std::istream& stm)
	{
		char uuid[64];
		stm.read(uuid, 64);
		__uuid = std::string(uuid, 64);
		int mt;
		stm >> mt;
		return stm;
	}
}
