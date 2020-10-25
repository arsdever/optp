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

#include "operations/node_uuid_getter_op.h"
#include "operation_result.h"

#define SETUP_ERROR_HANDLER( errc, handler ) if ( ec == asio::error:: errc ) { handler(); return; }

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("remote_node", sink);

namespace optp
{
	remote_node::remote_node(asio::ip::tcp::socket remote_socket, interfaces::node_def_shptr def)
		: m_socket(std::move(remote_socket))
		, m_definition(def)
		, m_address(def->address())
		, m_eventHandlers{}
	{
	}

	remote_node::~remote_node()
	{
		m_socket.close();
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

	interfaces::operation_result_shptr remote_node::handle(interfaces::operation_shptr op)
	{
		logger->info("Handling remote operation with uuid {0}", std::static_pointer_cast<operation>(op)->uuid());

		return std::make_shared<operation_result>(
			std::dynamic_pointer_cast<interfaces::object>(getDefinition().lock())->uuid(),
			std::dynamic_pointer_cast<interfaces::object>(op)->uuid());
	}

	interfaces::operation_result_shptr remote_node::handle_operation(interfaces::operation_shptr op)
	{
		//operation_result_shptr result = std::make_shared<operation_result>();

		return handle(op);
	}

	void remote_node::startup()
	{
		m_socket.async_read_some(asio::buffer(m_incomingMessageBuffer, 1024), std::bind(&remote_node::read_message, this, std::placeholders::_1, std::placeholders::_2));
	}

	void remote_node::set_event_handlers(event_handler_mapping const& ehm)
	{
		m_eventHandlers = ehm;
	}

	void remote_node::read_message(asio::error_code const& ec, std::size_t bytes)
	{
		if (ec)
		{
			SETUP_ERROR_HANDLER(eof, on_disconnect)
			SETUP_ERROR_HANDLER(connection_reset, on_connection_reset)

			logger->error("Failed to read the message [{}]: {}", ec.value(), ec.message());
			m_socket.close();
			return;
		}

		if (!bytes)
		{
			if (!bytes)
				logger->error("An empty message was read");

			m_socket.async_read_some(asio::buffer(m_incomingMessageBuffer, 1024), std::bind(&remote_node::read_message, this, std::placeholders::_1, std::placeholders::_2));
			return;
		}

		std::istringstream strm(m_incomingMessageBuffer);
		int type_id = std::stoi(std::string(m_incomingMessageBuffer, 5));
		interfaces::deserializable_shptr ptr = std::dynamic_pointer_cast<interfaces::deserializable>(object_metatypes::object_ctor_mapping::map[type_id]());
		ptr->deserialize(strm);
		interfaces::object_shptr optr = std::dynamic_pointer_cast<interfaces::object>(ptr);
		logger->info("Remote object \"{}\" received with uuid {}", object_metatypes::object_metatype_name((object_metatypes::object_metatype)optr->metatype()), optr->uuid());

		if (optr->metatype() == (int)object_metatypes::object_metatype::OPERATION)
		{
			handle(std::dynamic_pointer_cast<interfaces::operation>(optr));
		}
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
		if (handshake() && receive_handshake())
			startup();

		cb(shared_from_this());
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

	bool remote_node::handshake()
	{
		if (interfaces::optp_shptr shproto = m_protocol.lock())
		{
			if (interfaces::node_shptr thisnode = shproto->thisNode().lock())
			{
				if (interfaces::node_def_shptr thisnodedef = thisnode->getDefinition().lock())
				{
					std::stringstream strm;
					std::dynamic_pointer_cast<node_def>(thisnodedef)->serialize(strm);
					m_socket.write_some(asio::buffer(strm.str()));
					return true;
				}
				else
				{
					logger->warn("The current real node instance is missing it's definition");
					m_socket.close();
					return false;
				}
			}
			else
			{
				logger->warn("Protocol has not configured properly: The current node instance is missing");
				m_socket.close();
				return false;
			}
		}
		else
		{
			logger->warn("Protocol is not valid");
			m_socket.close();
			return false;
		}
	}

	bool remote_node::receive_handshake()
	{
		std::string buffer(128, '\0');
		asio::error_code error;
		m_socket.read_some(asio::buffer(buffer), error);
		if (error)
		{
			logger->error("Failed to receive the handshake [{}]: {}", error.value(), error.message());
			m_socket.close();
			return false;
		}

		buffer = buffer.substr(5);
		interfaces::deserializable_shptr des = std::dynamic_pointer_cast<interfaces::deserializable>(m_definition);
		std::stringstream bufstm(buffer);
		des->deserialize(bufstm);
		return true;
	}
	
	void remote_node::set_protocol(interfaces::optp_wptr proto)
	{
		m_protocol = proto;
	}
}
