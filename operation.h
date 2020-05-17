/*
 * =====================================================================================
 *
 *		Filename:	opeartion.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <interface/operation.h>
#include <unordered_set>

namespace optp
{
	class operation : public interfaces::operation
	{
	public:
		operation();
		std::string uuid() const override;
		std::string serialize() const override;
		void deserialize(std::string const& data) override;

	private:
		std::string m_uuid;
		std::unordered_set<interfaces::operation_shptr> m_nodeSpecOperation;
	};
}