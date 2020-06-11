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
#include <spdlog/spdlog.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("remote_node", sink);

namespace optp
{
	remote_node::remote_node(sockpp::tcp_socket&& remote_socket)
		: m_remoteSocket(std::move(remote_socket))
		, m_uuid(std::move(uuid_provider().provideRandomString()))
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
				interfaces::operation_shptr op = std::make_shared<operation>();
				std::string message(buffer, read_bytes);
				logger->info("Deserializing incoming message\n{0}", message);
				op->deserialize(message);
				logger->info("Remote operation received with uuid {0}", op->uuid());
				node.handle(op);
			}

			logger->info("Remote node disconnected {0}", node.address());
			// TODO: Callback must be executed to notify about disconnection
		}).detach();
	}
}