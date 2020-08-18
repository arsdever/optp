/*
 * =====================================================================================
 *
 *		Filename:	node_def.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "node_def.h"

namespace optp
{
	node_def::node_def()
		: m_address()
	{}

	node_def::node_def(std::string const& address)
		: m_address(address)
	{}

	std::string node_def::address() const
	{
		return m_address;
	}

	void node_def::setAddress(std::string const& address)
	{
		m_address = address;
	}

	std::ostream& node_def::serialize(std::ostream& stm) const
	{
		object::serialize(stm);
		return stm;
	}


	std::istream& node_def::deserialize(std::istream& stm)
	{
		object::deserialize(stm);
		return stm;
	}
}