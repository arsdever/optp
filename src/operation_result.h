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
	private:
		operation_result();

	public:
		operation_result(std::string const& node_uuid, std::string const& operation_uuid);
		std::string operationUuid() const override;
		std::string nodeUuid() const override;

		std::ostream& serialize(std::ostream& stm) const override;
		std::istream& deserialize(std::istream& stm) override;

		void setResultData(nlohmann::json const& data);
		nlohmann::json data() const;

		int metatype() const override { return (int)object_metatypes::object_metatype::OPERATION_RESULT; }

		inline static interfaces::object_shptr ctor() { return interfaces::object_shptr((interfaces::object*)new operation_result()); }

	private:
		std::string m_nodeUuid;
		std::string m_operationUuid;
		nlohmann::json m_data;
	};
}