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

#define DEFINE_DEFAULT_EVENT( e ) DEFINE_EVENT( e, std::function<void(interfaces::node_wptr)> )
#define DEFINE_EVENT( e, ft ) ft m_##e = {}; inline event_handler_mapping& e ( ft f ) { m_##e = f; return *this; }

namespace optp
{
	class remote_node
		: public interfaces::node
	{
		typedef std::function<void(interfaces::node_shptr)> on_connection_cb_t;

	public:
		struct event_handler_mapping
		{
			DEFINE_DEFAULT_EVENT(on_disconnect)
			DEFINE_DEFAULT_EVENT(on_connection_reset)
			DEFINE_DEFAULT_EVENT(on_connection_refused)
		};

	public:
		remote_node(/*interfaces::optp_wptr protocol, */asio::ip::tcp::socket socket, interfaces::node_def_shptr def = nullptr);
		~remote_node();

		std::string address() const override;
		interfaces::operation_shptr execute(interfaces::operation_shptr operation) override;
		interfaces::operation_shptr handle(interfaces::operation_shptr operation) override;
		interfaces::node_def_wptr getDefinition() const override;

		void set_event_handlers(event_handler_mapping const& ehm);

		void handshake();
		void connect(on_connection_cb_t cb);

	private:
		void startup();
		void read_message(asio::error_code const& ec, std::size_t bytes);

		// Event handlers
		void on_connected(asio::error_code const& ec, on_connection_cb_t cb);
		void on_disconnect();
		void on_connection_reset();

	private:
		asio::ip::tcp::socket m_socket;
		//interfaces::optp_wptr m_protocol;
		interfaces::node_def_shptr m_definition;
		std::string m_address;
		bool m_aboutToDisconnect;
		char m_incomingMessageBuffer[1024];
		std::vector<std::string> m_messages;
		event_handler_mapping m_eventHandlers;
	};
}