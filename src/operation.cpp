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
		: object(object_metatype::OPERATION)
		, __operation_type(-1)
	{
	}

	std::ostream& operation::serialize(std::ostream& stm) const
	{
		object::serialize(stm);
		stm << type();
		return stm;
	}

	std::istream& operation::deserialize(std::istream& stm)
	{
		object::deserialize(stm);
		stm >> __operation_type;
		return stm;
	}

	int operation::type() const
	{
		return __operation_type;
	}

	void operation::setType(int type)
	{
		__operation_type = type;
	}

	void operation::setResult(interfaces::operation_result_shptr result)
	{
		m_nodeSpecResult[result->nodeUuid()] = result;
	}

	std::list<interfaces::operation_result_wptr> operation::getResults() const
	{
		std::list<interfaces::operation_result_wptr> result_list;
		for (std::pair<std::string, interfaces::operation_result_shptr> const& result_pair : m_nodeSpecResult)
			result_list.push_back(result_pair.second);
		
		return result_list;
	}
}