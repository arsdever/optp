#pragma once

//#include <sockpp/tcp_acceptor.h>
#include <asio.hpp>
#include <optp/typedefs.h>

namespace optp
{
	class connector
	{
	public:
		//typedef std::function<void(sockpp::tcp_socket&& socket, sockpp::inet_address&& address)> on_connection_cb_t;
		typedef std::function<void(interfaces::node_shptr)> on_connection_cb_t;

	public:
		connector();
		~connector();
		void loop();

		//void onConnection(on_connection_cb_t cb);
		void start();
		void register_on_connection(on_connection_cb_t cb);
		interfaces::node_shptr connect_to(std::string const& address);

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
		asio::ip::tcp::acceptor m_server;
		on_connection_cb_t m_onConnectionCB;
		volatile bool alive;
	};
}
