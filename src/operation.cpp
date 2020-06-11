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
		return "operation";
	}

	void operation::deserialize(std::string const& data)
	{
		return;
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