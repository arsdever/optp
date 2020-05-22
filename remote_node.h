/*
 * =====================================================================================
 *
 *		Filename:	remote_node.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include <interface/node.h>
#include <sockpp/tcp_socket.h>

namespace optp
{
	class remote_node : public interfaces::node
	{
	public:
		remote_node(sockpp::tcp_socket&& remote_socket);

		// interfaces::node started
		std::string address() const override; // TODO: Should be changed to address type
		interfaces::operation_ref execute(interfaces::operation_ref operation) override;
		interfaces::operation_ref handle(interfaces::operation_ref operation) override;
		std::string uuid() const override;
		// interfaces::node finished

	private:
		void setupListener();

	private:
		sockpp::tcp_socket m_remoteSocket;
		std::string m_uuid;
	};
}