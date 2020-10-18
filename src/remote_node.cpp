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
#include <asio.hpp>
#include <asio/placeholders.hpp>

//#include <sockpp/tcp_socket.h>

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("remote_node", sink);

namespace optp
{
	remote_node::remote_node(/*interfaces::optp_wptr protocol, */asio::ip::tcp::socket remote_socket, interfaces::node_def_shptr def)
		: m_socket(std::move(remote_socket))
		//, m_protocol(protocol)
		, m_definition(def)
		, m_address(def->address())
		, m_eventHandlers{}
	{
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
		startup();
	}

	remote_node::~remote_node()
	{
		//logger->info("Closing the remote socket");
		//// Close the socket
		//m_remoteSocket.shutdown();
		//m_remoteSocket.close();
		//m_remoteSocket.destroy();

		//logger->info("Stopping the listener thread");
		//// Stop the listener loop

		//logger->info("Removing the remote_node associated with ip {0}", address());
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
		m_messages.push_back(message);
		std::vector<std::string>::const_iterator iterator = std::prev(m_messages.end());
		m_socket.async_write_some(asio::buffer(*iterator), [&messages = m_messages, iterator](asio::error_code const& ec, std::size_t bytes) {
			if (ec)
			{
				logger->error("Failed to write the message: {}", ec.message());
			}
			messages.erase(iterator);
			});
		return op;
	}

	interfaces::operation_shptr remote_node::handle(interfaces::operation_shptr op)
	{
		logger->info("Handling remote operation with uuid {0}", std::static_pointer_cast<operation>(op)->uuid());
		return op;
	}

	void remote_node::startup()
	{
		m_socket.async_read_some(asio::buffer(m_incomingMessageBuffer, 1024), std::bind(&remote_node::read_message, this, std::placeholders::_1, std::placeholders::_2));

		//while (!m_aboutToDisconnect && (read_bytes = m_remoteSocket.read(buffer, sizeof(buffer))) > 0)
		//{
		//	std::string message(buffer, read_bytes);
		//	logger->info("Incoming message\n\tFrom node:\n\t\tNode UUID: {0}\n\t\tNode address: {1}\n\t\tMessage: {2}",
		//		std::dynamic_pointer_cast<interfaces::object>(m_definition)->uuid(),
		//		address(),
		//		message);
		//	std::istringstream strm(message);
		//	std::istringstream type_id_stream(message.substr(0, 5));

		//	int type_id;
		//	type_id_stream >> type_id;
		//	interfaces::deserializable_shptr ptr = std::dynamic_pointer_cast<interfaces::deserializable>(object_metatypes::object_ctor_mapping::map[type_id]());

		//	ptr->deserialize(strm);
		//	interfaces::object_shptr optr = std::dynamic_pointer_cast<interfaces::object>(ptr);
		//	logger->info("Remote operation received with uuid {0}", optr->uuid());
		//	//handle(optr);
		//}

		//logger->info("Remote node disconnected {0}", address());
		//m_protocol.lock()->disconnectFromNode(shared_from_this());
	}

	void remote_node::set_event_handlers(event_handler_mapping const& ehm)
	{
		m_eventHandlers = ehm;
	}

	void remote_node::read_message(asio::error_code const& ec, std::size_t bytes)
	{
		if (ec)
		{
			if (ec == asio::error::eof)
			{
				on_disconnect();
				return;
			}

			if (ec == asio::error::connection_reset)
			{
				on_connection_reset();
				return;
			}

			logger->error("Failed to read the message: {}", ec.message());
			//asio::async_read(m_socket, asio::buffer(data, 1024), std::bind(&remote_node::read_message, this, std::placeholders::_1, std::placeholders::_2, std::move(data)));
			return;
		}

		if (bytes == 0)
		{
			m_socket.async_read_some(asio::buffer(m_incomingMessageBuffer, 1024), std::bind(&remote_node::read_message, this, std::placeholders::_1, std::placeholders::_2));
			return;
		}

		int type_id;
		std::istringstream strm(m_incomingMessageBuffer);
		strm >> type_id;
		interfaces::deserializable_shptr ptr = std::dynamic_pointer_cast<interfaces::deserializable>(object_metatypes::object_ctor_mapping::map[type_id]());
		ptr->deserialize(strm);
		interfaces::object_shptr optr = std::dynamic_pointer_cast<interfaces::object>(ptr);
		logger->info("Remote operation received with uuid {0}", optr->uuid());
		m_socket.async_read_some(asio::buffer(m_incomingMessageBuffer, 1024), std::bind(&remote_node::read_message, this, std::placeholders::_1, std::placeholders::_2));
	}

	void remote_node::connect(on_connection_cb_t cb)
	{
		asio::ip::tcp::endpoint ep{ asio::ip::address::from_string(address()), 50000 };
		m_socket.async_connect(ep, [this, cb](asio::error_code const& code) {
			on_connected(code, cb);
			});
	}


	void remote_node::on_connected(asio::error_code const& ec, on_connection_cb_t cb)
	{
		if (ec)
		{
			logger->warn("Failed to connect to the server: {}", ec.message());
			return;
		}

		logger->info("Successfully connected to remote");
		startup();
		/*std::string buffer;
		asio::error_code error;
		socket.read_some(asio::buffer(buffer), error);
		if (error)
		{
			logger->error("Couldn't read from remote");
			return;
		}

		node_def_shptr def = std::make_shared<node_def>(socket.remote_endpoint().address().to_string());
		rnode_shptr remote = std::make_shared<remote_node>(std::move(socket), std::move(def));
		cb(std::move(remote));*/
	}

	void remote_node::on_disconnect()
	{
		logger->info("Remote node was disconnected");
		if (m_eventHandlers.m_on_disconnect)
			m_eventHandlers.m_on_disconnect(shared_from_this());
	}

	void remote_node::on_connection_reset()
	{
		logger->info("Remote connection was reset");
		if (m_eventHandlers.m_on_connection_reset != nullptr)
			m_eventHandlers.m_on_connection_reset(shared_from_this());
	}

	interfaces::node_def_wptr remote_node::getDefinition() const
	{
		return m_definition;
	}

	void remote_node::handshake()
	{
		m_socket.write_some(asio::buffer(std::dynamic_pointer_cast<interfaces::object>(m_definition)->uuid()));
	}
}
