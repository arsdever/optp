/*
 * =====================================================================================
 *
 *		Filename:	node_def.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp/node_def.h>
#include "object.h"

namespace optp
{
	class node_def
		: public interfaces::node_def
		, public object
	{
	private:
		node_def();

	public:
		node_def(std::string const& address);

		std::string address() const override;
		void setAddress(std::string const& address) override;
		std::ostream& serialize(std::ostream& stm) const override;
		std::istream& deserialize(std::istream& stm) override;

		int metatype() const override { return (int)object_metatypes::object_metatype::NODE_DEF; }

		inline static interfaces::object_shptr ctor() { return interfaces::object_shptr((interfaces::object*)new node_def()); }

	private:
		
		std::string m_address;
	};
}