#pragma once

//#include <sockpp/tcp_acceptor.h>
#include <asio.hpp>
#include <optp/typedefs.h>
#include "remote_node.h"

namespace optp
{
	class connector
	{
	public:
		//typedef std::function<void(sockpp::tcp_socket&& socket, sockpp::inet_address&& address)> on_connection_cb_t;
		typedef std::function<void(interfaces::node_shptr)> on_connection_cb_t;

	public:
		connector(interfaces::optp_wptr protocol);
		~connector();
		void loop();

		//void onConnection(on_connection_cb_t cb);
		void start();
		void register_on_connection(on_connection_cb_t cb);
		void set_event_handler_mapping(remote_node::event_handler_mapping ehm);
		void connect_to(std::string const& address, on_connection_cb_t connectedCB);

	private:
		void on_connection(asio::error_code const& ec, asio::ip::tcp::socket peer);
		void on_connected(asio::error_code const& ec, asio::ip::tcp::socket socket, on_connection_cb_t cb);

		void start_accept();

	private:
		/// <summary>
		/// The time point when the instance was created
		/// </summary>
		/// 
		/// Used for nodes to determine which one is the main node
		const int m_startupTimestamp;

		// The asio context
		asio::io_context m_asioContext;
		std::thread m_asioHandlerThread;

		//std::thread m_listenerThread;
		interfaces::optp_wptr m_protocol;
		asio::ip::tcp::acceptor m_server;
		on_connection_cb_t m_onConnectionCB;
		remote_node::event_handler_mapping m_eventHandlerMapping;
		volatile bool alive;
		std::unordered_set<interfaces::node_shptr> m_pendingConnections;
	};
}
