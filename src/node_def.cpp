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
#include <sstream>

namespace optp
{
	node_def::node_def(std::string const& address)
		: m_address(address)
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

	std::string node_def::serialize() const
	{
		return std::string("node_def ") + uuid() + " " + address();
	}

	void node_def::deserialize(std::string const& dataBuffer)
	{
		std::istringstream iss(dataBuffer);
		std::vector<std::string> result{
			std::istream_iterator<std::string>(iss), {}
		};

		if (result.size() != 3)
			return;

		m_uuid = result[1];
		m_address = result[2];
	}
}