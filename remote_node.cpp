/*
 * =====================================================================================
 *
 *		Filename:	remote_node.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "remote_node.h"
#include "uuid_provider.h"
#include "operation.h"
#include <thread>

namespace optp
{
	remote_node::remote_node(sockpp::tcp_socket&& remote_socket)
		: m_remoteSocket(std::move(remote_socket))
		, m_uuid(std::move(uuid_provider().provideRandomString()))
	{}

	std::string remote_node::address() const
	{
		return m_remoteSocket.address().to_string();
	}

	interfaces::operation& remote_node::execute(interfaces::operation& operation)
	{
		m_remoteSocket.write(operation.serialize());
		return operation;
	}

	interfaces::operation& remote_node::handle(interfaces::operation& operation)
	{
		m_remoteSocket.write(operation.serialize());
		return operation;
	}

	std::string remote_node::uuid() const
	{
		return m_uuid;
	}

	void remote_node::setupListener()
	{
		std::thread([&node = *this, &peer_socket = m_remoteSocket]() {
			char buffer[1024];
			size_t read_bytes;

			while ((read_bytes = peer_socket.read(buffer, sizeof(buffer))) > 0)
			{
				operation op;
				op.deserialize(std::string(buffer, read_bytes));
				node.handle(op);
			}

			// connecion closed
			// TODO: print connection closed log
			}
		).detach();
	}
}