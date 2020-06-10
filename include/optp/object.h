/*
 * =====================================================================================
 *
 *		Filename:	optp/object.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

namespace optp
{
	class uuid;

	namespace interface
	{
		class object
		{
		private:
			object(object const& other) = delete;
			object& operator = (object const& other) = delete;

		public:
			virtual ~object() = default;
			virtual uuid uuid() const = 0;
			virtual object clone() const = 0;
		};
	}
}
