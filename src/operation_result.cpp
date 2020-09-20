/*
 * =====================================================================================
 *
 *		Filename:	operation_result.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "operation_result.h"

namespace optp
{
	operation_result::operation_result()
		: m_nodeUuid()
		, m_operationUuid()
	{}

	operation_result::operation_result(std::string const& node_uuid, std::string const& operation_uuid)
		: m_nodeUuid(node_uuid)
		, m_operationUuid(operation_uuid)
	{}

	std::string operation_result::operationUuid() const
	{
		return m_operationUuid;
	}

	std::string operation_result::nodeUuid() const
	{
		return m_nodeUuid;
	}

	std::ostream& operation_result::serialize(std::ostream& stm) const
	{
		return object::serialize(stm) << m_data;
	}

	std::istream& operation_result::deserialize(std::istream& stm)
	{
		return object::deserialize(stm) >> m_data;
	}

	void operation_result::setResultData(nlohmann::json const& data)
	{
		m_data = data;
	}

	nlohmann::json operation_result::data() const
	{
		return m_data;
	}
}