/*
 * =====================================================================================
 *
 *		Filename:	node_def.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "node_def.h"
#include "uuid_provider.h"

namespace optp
{
	node_def::node_def(std::string const& address)
		: object(object_metatype::NODE_DEF)
		, m_address(address)
		, m_uuid(uuid_provider().provideRandomString())
	{}

	std::string node_def::address() const
	{
		return m_address;
	}

	std::string node_def::uuid() const
	{
		return m_uuid;
	}

	std::ostream& node_def::serialize(std::ostream& stm) const
	{
		object::serialize(stm);
		stm << address();
		return stm;
	}

	std::istream& node_def::deserialize(std::istream& stm)
	{
		stm >> m_address;
		return stm;
	}
}