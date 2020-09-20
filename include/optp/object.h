/*
 * =====================================================================================
 *
 *		Filename:	optp/object.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#if !__has_include(<optp/object_metatypes.h>)
#error The project seems to be broken. Some important files are missing. Please regenerate the solution using CMake.
#else
#include <optp/object_metatypes.h>
#include <optp/typedefs.h>

namespace optp
{
	namespace interfaces
	{
		class object : public std::enable_shared_from_this<object>
		{
		private:
			object(object const& other) = delete;
			object& operator = (object const& other) = delete;

		public:
			object() = default;
			virtual ~object() = default;
			virtual std::string uuid() const = 0;
			//virtual object_shptr clone() const = 0;
			virtual int metatype() const = 0;
		};
	}
}
#endif
