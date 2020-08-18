/*
 * =====================================================================================
 *
 *		Filename:	opeartion.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp/operation.h>
#include <unordered_map>

#include <optp_export.h>

#include "object.h"

namespace optp
{
	class OPTP_EXPORT operation
		: public interfaces::operation
		, public object
	{
	private:
		operation();

	public:
		operation(int type);
		int type() const override;
		void setResult(interfaces::operation_result_shptr result) override;
		std::list<interfaces::operation_result_wptr> getResults() const override;

		std::ostream& serialize(std::ostream& stm) const override;
		std::istream& deserialize(std::istream& stm) override;

		int metatype() const override { return (int)object_metatypes::object_metatype::OPERATION; }

		inline static interfaces::object_shptr ctor() { return interfaces::object_shptr((interfaces::object*)new operation()); }

	protected:
		void setType(int type) override;

	private:
		std::unordered_map<std::string, interfaces::operation_result_shptr> m_nodeSpecResult;
		int __operation_type;
	};
}