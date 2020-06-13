/*
 * =====================================================================================
 *
 *		Filename:	real_node.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include <optp/node.h>
#include <list>
#include <chrono>

namespace optp
{
	class real_node : public interfaces::node
	{
	public:
		real_node();

		std::string address() const override;
		interfaces::operation_shptr execute(interfaces::operation_shptr operation) override;
		interfaces::operation_shptr handle(interfaces::operation_shptr operation) override;
		std::string uuid() const override;

		void registerRemoteNode(interfaces::node_wptr remote_node);
		static std::string getLocalIpAddress();

	private:
		std::string m_uuid;
		std::string m_ipAddress; // TODO: Use lib specific ip address structure
		std::list<interfaces::node_wptr> m_remoteNodes;
	};
}
