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

namespace optp
{
	class OPTP_EXPORT operation : public interfaces::operation
	{
	public:
		operation();
		std::string uuid() const override;
		int type() const override;
		std::string serialize() const override;
		void deserialize(std::string const& data) override;
		void setResult(interfaces::operation_result_shptr result) override;

	private:
		std::string m_uuid;
		std::unordered_map<std::string, interfaces::operation_result_shptr> m_nodeSpecResult;
	};
}