/*
 * =====================================================================================
 *
 *       Filename:  object.h
 *         Author:  Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp/object.h>
#include <optp/serializable.h>
#include <optp/deserializable.h>

namespace optp
{
	class object
		: public interfaces::object
		, public interfaces::serializable
		, public interfaces::deserializable
	{
	public:
		object();

		std::string uuid() const { return __uuid; }
		virtual int metatype() const = 0;

		std::ostream& serialize(std::ostream& stm) const override;
		std::istream& deserialize(std::istream& stm) override;

	private:
		std::string __uuid;
	};
}
