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

namespace optp
{
	class remote_node : public interfaces::node
	{
	public:
		remote_node(optp_wptr protocol, sockpp::tcp_socket&& remote_socket);

		std::string address() const override; // TODO: Should be changed to address type
		interfaces::operation_shptr execute(interfaces::operation_shptr operation) override;
		interfaces::operation_shptr handle(interfaces::operation_shptr operation) override;
		std::string uuid() const override;

	private:
		void setupListener();

	private:
		sockpp::tcp_socket m_remoteSocket;
		std::string m_uuid;
		optp_wptr m_protocol;
	};
}