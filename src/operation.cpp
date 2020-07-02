/*
 * =====================================================================================
 *
 *		Filename:	opeartion.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "operation.h"
#include "uuid_provider.h"
#include <optp/operation_result.h>

#include <sstream>

namespace optp
{
	operation::operation()
		: m_uuid(uuid_provider().provideRandomString())
	{
	}

	std::string operation::uuid() const
	{
		return m_uuid;
	}

	std::string operation::serialize() const
	{
		return std::string("operation ") + uuid();
	}

	void operation::deserialize(std::string const& data)
	{
		std::istringstream iss(data);
		std::vector<std::string> result{
			std::istream_iterator<std::string>(iss), {}
		};

		if (result.size() < 2)
			return;

		m_uuid = result[1];
	}

	int operation::type() const
	{
		return -1;
	}

	void operation::setResult(interfaces::operation_result_shptr result)
	{
		//m_nodeSpecResult[result->nodeUuid()] = result;
	}
}