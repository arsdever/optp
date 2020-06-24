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
#include "node_def.h"

#include <optp/optp.h>

#include <thread>

#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("remote_node", sink);

namespace optp
{
	remote_node::remote_node(interfaces::optp_wptr protocol, sockpp::tcp_socket&& remote_socket, interfaces::node_def_shptr def)
		: m_remoteSocket(std::move(remote_socket))
		, m_uuid(uuid_provider().provideRandomString())
		, m_protocol(protocol)
		, m_definition(nullptr)
	{
		setupListener();
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
	}

	std::string remote_node::address() const
	{
		return m_remoteSocket.address().to_string();
	}

	interfaces::operation_shptr remote_node::execute(interfaces::operation_shptr operation)
	{
		logger->info("Sending operation with uuid {0} to remote", operation->uuid());
		std::string message = operation->serialize();
		size_t bytes_written = m_remoteSocket.write(message);
		logger->info("{0} bytes were sent\n{1}", bytes_written, message);
		return operation;
	}

	interfaces::operation_shptr remote_node::handle(interfaces::operation_shptr operation)
	{
		logger->info("Handling remote operation with uuid {0}", operation->uuid());
		if (interfaces::optp_shptr protocol = m_protocol.lock())
		{
			return protocol->handle(operation);
		}

		return operation;
	}

	std::string remote_node::uuid() const
	{
		return m_uuid;
	}

	void remote_node::setupListener()
	{
		std::thread{ &remote_node::listener, this }.detach();
	}

	void remote_node::listener()
	{
		char buffer[1024];
		size_t read_bytes;

		while ((read_bytes = m_remoteSocket.read(buffer, sizeof(buffer))) > 0)
		{
			std::string message(buffer, read_bytes);

			if (m_definition == nullptr)
			{
				logger->info("node_def received from node with address {0}\n\t{1}", m_remoteSocket.address().to_string(), message);
				m_definition = std::move(std::make_shared<node_def>(""));
				m_definition->deserialize(message);

				logger->info("Trying to respond with current node's definition");
				if (const interfaces::optp_shptr protocol = m_protocol.lock())
				{
					if (const interfaces::node_shptr local_node = protocol->thisNode().lock())
					{
						if (const interfaces::node_def_shptr local_node_def = local_node->getDefinition().lock())
						{
							std::string def_message = local_node_def->serialize();
							logger->info("Sending the node information\n\t{0}", def_message);
							m_remoteSocket.write(def_message);
						}
					}
				}
				continue;
			}

			interfaces::operation_shptr op = std::make_shared<operation>();
			logger->info("Deserializing incoming message\n{0}", message);
			op->deserialize(message);
			logger->info("Remote operation received with uuid {0}", op->uuid());
			handle(op);
		}

		logger->info("Remote node disconnected {0}", address());
		// TODO: Callback must be executed to notify about disconnection
	}
	
	void remote_node::setProtocol(interfaces::optp_wptr protocol)
	{
		m_protocol = protocol;
	}

	interfaces::node_def_wptr remote_node::getDefinition() const
	{
		return m_definition;
	}
}