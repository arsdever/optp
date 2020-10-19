#include "connector.h"

#include <optp/optp.h>
#include <sstream>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/spdlog.h>
#include "remote_node.h"
#include "node_def.h"

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("connection_listener", sink);

extern const int OPTP_DEFAULT_PORT = 33000; // TODO: must be configurable

namespace optp
{
	connector::connector(interfaces::optp_wptr protocol)
		: m_protocol{ protocol }
		, m_startupTimestamp(std::chrono::system_clock::now().time_since_epoch().count())
		, m_server(m_asioContext, { asio::ip::tcp::v4(), 50000 })
		//, m_listenerThread{ &connection_listener::loop, this }
		, alive{ true }
	{
		//logger->info("Local server started on {0}", static_cast<sockpp::inet_address>(m_serverSocket.address()).to_string());
		//if (!m_serverSocket)
		//{
			// TODO: print error log
			//return;
		//}

		// Create a main server thread to listen for incoming connections.
	}

	connector::~connector()
	{
		//// Close the socket
		//logger->info("Closing the listener socket");
		//m_serverSocket.shutdown();
		//m_serverSocket.close();
		//m_serverSocket.destroy();

		//logger->info("Stopping the listener thread");
		//// Stop the listener loop
		//alive = false;
		//m_listenerThread.join();

		//logger->info("Listener is about to be deleted");

		m_server.close();
		m_asioContext.stop();
		m_asioHandlerThread.join();
	}

	void connector::start()
	{
		m_server.listen();
		start_accept();
		m_asioHandlerThread = std::thread([this] { m_asioContext.run(); });
		logger->info("Server started");
	}

	void connector::on_connection(asio::error_code const& ec, asio::ip::tcp::socket peer)
	{
		start_accept();

		if (ec)
		{
			logger->warn("Failed to accept the connection: {}", ec.message());
			return;
		}

		logger->info("Remote peer connected successfully");
		node_def_shptr def = std::make_shared<node_def>("127.0.0.1");
		rnode_shptr rnode = std::make_shared<remote_node>(std::move(peer), std::move(def));
		rnode->set_protocol(m_protocol);
		rnode->set_event_handlers(m_eventHandlerMapping);
		if (rnode->receive_handshake() && rnode->handshake())
		{
			rnode->startup();
			m_onConnectionCB(std::move(rnode));
		}
	}

	void connector::connect_to(std::string const& address, on_connection_cb_t connectedCB)
	{
		asio::ip::tcp::socket skt(m_asioContext);
		rnode_shptr rnode = std::make_shared<remote_node>(std::move(skt), std::make_shared<node_def>(address));
		m_pendingConnections.insert(rnode);
		rnode->set_event_handlers(m_eventHandlerMapping);
		rnode->set_protocol(m_protocol);
		rnode->connect([&pendingList = m_pendingConnections, connectedCB](interfaces::node_shptr node)
			{
				pendingList.erase(node);
				connectedCB(node);
			});
	}

	void connector::set_event_handler_mapping(remote_node::event_handler_mapping ehm)
	{
		m_eventHandlerMapping = ehm;
	}

	void connector::on_connected(asio::error_code const& ec, asio::ip::tcp::socket socket, on_connection_cb_t cb)
	{
		if (ec)
		{
			logger->warn("Failed to connect to the server: {}", ec.message());
			return;
		}

		logger->info("Successfully connected to remote");
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

	void connector::start_accept()
	{
		m_server.async_accept(std::bind(&connector::on_connection, this, std::placeholders::_1, std::placeholders::_2));
	}

	void connector::register_on_connection(on_connection_cb_t cb)
	{
		m_onConnectionCB = cb;
	}

	void connector::loop()
	{
	/*	while (alive)
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
		}*/
	}

	//void connection_listener::onConnection(on_connection_cb_t cb) { m_onConnectionCB = cb; }
}
