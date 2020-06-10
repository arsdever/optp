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
		typedef void (*execution_finished_cb) (interfaces::operation_ref);

	public:
		real_node();

		// interfaces::node started
		std::string address() const override;
		interfaces::operation_ref execute(interfaces::operation_ref operation) override;
		interfaces::operation_ref handle(interfaces::operation_ref operation) override;
		std::string uuid() const override;
		// interfaces::node finished

		void registerRemoteNode(interfaces::node_wptr remote_node);
		static std::string getLocalIpAddress();

	private:
		std::string m_uuid;
		std::string m_ipAddress; // TODO: Use lib specific ip address structure
		std::list<interfaces::node_wptr> m_remoteNodes;
	};
}
