/*
 * =====================================================================================
 *
 *		Filename:	remote_node.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "remote_node.h"
#include "operation.h"
#include "node_def.h"
#include "typedefs.h"

#include <optp/optp.h>

#include <thread>
#include <sstream>

#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>

#include <sockpp/tcp_socket.h>

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("remote_node", sink);

namespace optp
{
	remote_node::remote_node(interfaces::optp_wptr protocol, sockpp::tcp_socket&& remote_socket, interfaces::node_def_shptr def)
		: m_remoteSocket(std::move(remote_socket))
		, m_protocol(protocol)
		, m_definition(def)
		, m_address(def->address())
	{
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
		std::thread{ &remote_node::listener, this }.detach();
	}

	remote_node::~remote_node()
	{
		logger->info("Closing the remote socket");
		// Close the socket
		m_remoteSocket.shutdown();
		m_remoteSocket.close();
		m_remoteSocket.destroy();

		logger->info("Stopping the listener thread");
		// Stop the listener loop

		logger->info("Removing the remote_node associated with ip {0}", address());
	}

	std::string remote_node::address() const
	{
		return m_address;
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

	void remote_node::listener()
	{
		char buffer[1024];
		int read_bytes;

		while ((read_bytes = m_remoteSocket.read(buffer, sizeof(buffer))) > 0)
		{
			std::string message(buffer, read_bytes);
			logger->info("Incoming message\n\tFrom node:\n\t\tNode UUID: {0}\n\t\tNode address: {1}\n\t\tMessage: {2}",
				std::dynamic_pointer_cast<interfaces::object>(m_definition)->uuid(),
				address(),
				message);
			std::istringstream strm(message);
			std::istringstream type_id_stream(message.substr(0, 5));

			int type_id;
			type_id_stream >> type_id;
			interfaces::deserializable_shptr ptr = std::dynamic_pointer_cast<interfaces::deserializable>(object_metatypes::object_ctor_mapping::map[type_id]());

			ptr->deserialize(strm);
			interfaces::object_shptr optr = std::dynamic_pointer_cast<interfaces::object>(ptr);
			logger->info("Remote operation received with uuid {0}", optr->uuid());
			//handle(optr);
		}

		logger->info("Remote node disconnected {0}", address());
	}

	interfaces::node_def_wptr remote_node::getDefinition() const
	{
		return m_definition;
	}
}
