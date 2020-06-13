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

namespace optp
{
	class operation_result : public interfaces::operation_result
	{
	public:
		operation_result(std::string const& node_uuid);
		std::string nodeUuid() const override;
		std::string uuid() const override;
		std::string serialize() const override;
		void deserialize(std::string const& data) override;

	private:
		std::string m_nodeUuid;
		std::string m_uuid;
	};
}