/*
 * =====================================================================================
 *
 *		Filename:	uuid_provider.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <string>

namespace optp
{
	class uuid_provider
	{
		struct srander
		{
			srander();
		};

	public:
		uuid_provider();
		std::string provideRandomString(size_t length = 32);
	};
}
