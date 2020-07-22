/*
 * =====================================================================================
 *
 *		Filename:	operation_result.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */
#pragma once

#include <optp/operation_result.h>

#include "object.h"

namespace optp
{
	class operation_result
		: public interfaces::operation_result
		, public object
	{
	public:
		operation_result(std::string const& node_uuid, std::string const& operation_uuid);
		std::string operationUuid() const override;
		std::string nodeUuid() const override;
		std::string uuid() const override;

		std::ostream& serialize(std::ostream& stm) const override;
		std::istream& deserialize(std::istream& stm) override;

	private:
		std::string m_nodeUuid;
		std::string m_operationUuid;
		std::string m_uuid;
	};
}