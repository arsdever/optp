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

#include "object.h"

namespace optp
{
	class remote_node
		: public interfaces::node
	{
	public:
		remote_node(interfaces::optp_wptr protocol, sockpp::tcp_socket&& remote_socket, interfaces::node_def_shptr def = nullptr);
		~remote_node();

		std::string address() const override;
		interfaces::operation_shptr execute(interfaces::operation_shptr operation) override;
		interfaces::operation_shptr handle(interfaces::operation_shptr operation) override;
		interfaces::node_def_wptr getDefinition() const override;

	private:
		void listener();

	private:
		sockpp::tcp_socket m_remoteSocket;
		interfaces::optp_wptr m_protocol;
		interfaces::node_def_shptr m_definition;
		std::string m_address;
	};
}