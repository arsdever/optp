/*
 * =====================================================================================
 *
 *		Filename:	uuid_provider.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "uuid_provider.h"

#include <ctime>
#include <random>

namespace optp
{
	std::string generateUuid(size_t length)
	{
		return uuid_provider().provideRandomString(length);
	}

	uuid_provider::srander::srander()
	{
		srand(time(0));
		rand();
	}

	uuid_provider::uuid_provider()
	{
		static srander srander;
	}

	std::string uuid_provider::provideRandomString(size_t length)
	{
		static const char characters[] = "0123456789qwertyuiopasdfghjklzxcvbnm";

		std::string uuid = "";
		for (int i = 0; i < length; ++i) {
			uuid.push_back(characters[rand() % 35]);
		}

		return uuid;
	}
}
