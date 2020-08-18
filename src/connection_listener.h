#pragma once

#include <sockpp/tcp_acceptor.h>
#include <thread>

namespace optp
{
	class connection_listener
	{
	public:
		typedef std::function<void(sockpp::tcp_socket&& socket, sockpp::inet_address&& address)> on_connection_cb_t;

	public:
		connection_listener();
		~connection_listener();
		void loop();

		void onConnection(on_connection_cb_t cb);

	private:
		sockpp::acceptor m_serverSocket;
		std::thread m_listenerThread;
		on_connection_cb_t m_onConnectionCB;
		volatile bool alive;
	};
}
