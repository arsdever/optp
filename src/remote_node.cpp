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
#include "typedefs.h"

#include <optp/optp.h>

#include <thread>
#include <sstream>

#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("remote_node", sink);

namespace optp
{
	remote_node::remote_node(interfaces::optp_wptr protocol, sockpp::tcp_socket&& remote_socket, interfaces::node_def_shptr def)
		: object(object_metatype::NODE)
		, m_remoteSocket(std::move(remote_socket))
		, m_protocol(protocol)
		, m_definition(def)
	{
		setupListener();
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
	}

	std::string remote_node::address() const
	{
		return m_remoteSocket.address().to_string();
	}

	interfaces::operation_shptr remote_node::execute(interfaces::operation_shptr op)
	{
		logger->info("Sending operation with uuid {0} to remote", std::static_pointer_cast<operation>(op)->uuid());
		std::stringstream strm;
		std::static_pointer_cast<operation>(op)->serialize(strm);
		std::string message = strm.str();
		size_t bytes_written = m_remoteSocket.write(message);
		logger->info("{0} bytes were sent\n{1}", bytes_written, message);
		return op;
	}

	interfaces::operation_shptr remote_node::handle(interfaces::operation_shptr op)
	{
		logger->info("Handling remote operation with uuid {0}", std::static_pointer_cast<operation>(op)->uuid());
		return op;
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

			operation_shptr op = std::make_shared<operation>();
			logger->info("Deserializing incoming message\n{0}", message);

			std::stringstream strm(message);
			op->deserialize(strm);
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
