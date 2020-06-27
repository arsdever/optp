/*
 * =====================================================================================
 *
 *		Filename:	node_def.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp/node_def.h>

namespace optp
{
	class node_def : public interfaces::node_def
	{
	public:
		node_def(std::string const& address);

		std::string address() const override;
		std::string uuid() const override;

		std::string serialize() const override;
		void deserialize(std::string const& dataBuffer) override;

	private:
		std::string m_uuid;
		std::string m_address;
	};
}