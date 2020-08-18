/*
 * =====================================================================================
 *
 *		Filename:	remote_node.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include <optp/node.h>
#include <sockpp/tcp_socket.h>

#include "object.h"

namespace optp
{
	class remote_node
		: public interfaces::node
	{
	public:
		remote_node(interfaces::optp_wptr protocol, sockpp::tcp_socket&& remote_socket, interfaces::node_def_shptr def = nullptr);

		std::string address() const override; // TODO: Should be changed to address type
		interfaces::operation_shptr execute(interfaces::operation_shptr operation) override;
		interfaces::operation_shptr handle(interfaces::operation_shptr operation) override;
		void setProtocol(interfaces::optp_wptr protocol) override;
		interfaces::node_def_wptr getDefinition() const override;

	private:
		void listener();
		void setupListener();

	private:
		sockpp::tcp_socket m_remoteSocket;
		interfaces::optp_wptr m_protocol;
		interfaces::node_def_shptr m_definition;
	};
}