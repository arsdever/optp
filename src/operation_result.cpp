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
	operation_result::operation_result(std::string const& node_uuid, std::string const& operation_uuid)
		: object(object_metatype::OPERATION_RESULT)
		, m_nodeUuid(node_uuid)
		, m_operationUuid(operation_uuid)
		, m_uuid(uuid_provider().provideRandomString())
	{}

	std::string operation_result::operationUuid() const
	{
		return m_operationUuid;
	}

	std::string operation_result::nodeUuid() const
	{
		return m_nodeUuid;
	}

	std::string operation_result::uuid() const
	{
		return m_uuid;
	}

	std::ostream& operation_result::serialize(std::ostream& stm) const
	{
		return object::serialize(stm);
	}

	std::istream& operation_result::deserialize(std::istream& stm)
	{
		return object::deserialize(stm);
	}
}