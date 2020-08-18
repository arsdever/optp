#include "connection_listener.h"

#include <optp/optp.h>
#include <sstream>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/spdlog.h>

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("connection_listener", sink);

extern const int OPTP_DEFAULT_PORT = 33000; // TODO: must be configurable

namespace optp
{
	connection_listener::connection_listener()
		: m_serverSocket{ std::move(sockpp::tcp_acceptor(OPTP_DEFAULT_PORT, 10)) }
		, m_listenerThread{ &connection_listener::loop, this }
		, alive{ true }
	{
		logger->info("Local server started on {0}", static_cast<sockpp::inet_address>(m_serverSocket.address()).to_string());
		if (!m_serverSocket)
		{
			// TODO: print error log
			return;
		}

		// Create a main server thread to listen for incoming connections.
	}

	connection_listener::~connection_listener()
	{
		// Close the socket
		logger->info("Closing the listener socket");
		m_serverSocket.shutdown();
		m_serverSocket.close();
		m_serverSocket.destroy();

		logger->info("Stopping the listener thread");
		// Stop the listener loop
		alive = false;
		m_listenerThread.join();

		logger->info("Listener is about to be deleted");
	}

	void connection_listener::loop()
	{
		while (alive)
		{
			sockpp::inet_address peer_address;
			sockpp::tcp_socket peer_socket = m_serverSocket.accept(&peer_address);

			logger->info("Peer is connecting with address {0}", peer_address.to_string());
			if (!peer_socket)
			{
				logger->error("Peer socket connection problem\n\t{0}", peer_socket.last_error_str());
				continue;
			}

			std::invoke(m_onConnectionCB, std::move(peer_socket), std::move(peer_address));
		}
	}

	void connection_listener::onConnection(on_connection_cb_t cb) { m_onConnectionCB = cb; }
}
