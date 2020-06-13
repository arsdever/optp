/*
 * =====================================================================================
 *
 *		Filename:	operation_result.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "operation_result.h"

#include "uuid_provider.h"

namespace optp
{
	operation_result::operation_result(std::string const& nodeUuid)
		: m_nodeUuid(nodeUuid)
		, m_uuid(uuid_provider().provideRandomString())
	{}

	std::string operation_result::nodeUuid() const
	{
		return m_nodeUuid;
	}

	std::string operation_result::uuid() const
	{
		return m_uuid;
	}

	std::string operation_result::serialize() const
	{
		return "operation_result";
	}

	void operation_result::deserialize(std::string const& data)
	{
		return;
	}
}