/*
 * =====================================================================================
 *
 *		Filename:	optp.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "optp.h"

#include "remote_node.h"
#include "real_node.h"
#include "network_interfaces.h"

#include <optp/node_def.h>

#include <thread>	

#include <sockpp/tcp_acceptor.h>
#include <sockpp/tcp_connector.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("optp", sink);

static const int OPTP_DEFAULT_PORT = 33000; // TODO: must be configurable
static sockpp::socket_initializer sockppInit;

namespace optp
{
	optp::optp(std::string const& config_file_path, interfaces::node_shptr node)
		: cm_maxConnectionCount(0)
		, m_configuration(optp_config::parse(config_file_path))
		, m_thisNode(node)
	{
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
		startServer();
		connectToServer();
	}

	optp::optp(std::string const& config_file_path)
		: cm_maxConnectionCount(0)
		, m_configuration(optp_config::parse(config_file_path))
	{
		m_thisNode = std::move(std::make_shared<real_node>(weak_from_this()));
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
		startServer();
		connectToServer();
	}

	optp::~optp()
	{}

	interfaces::node_wptr optp::thisNode() const
	{
		return m_thisNode;
	}

	interfaces::operation_shptr optp::execute(interfaces::operation_shptr operation)
	{
		// TODO: Make this to work in parallel for all nodes
		m_thisNode->handle(operation);
		for (interfaces::node_shptr remote : m_remotes)
		{
			remote->execute(operation);
		}
		return operation;
	}

	interfaces::operation_shptr optp::handle(interfaces::operation_shptr operation)
	{
		return m_thisNode->handle(operation);
	}

	void optp::connectToNode(std::string const& node_def)
	{
		if (network_interfaces::global().is_local(node_def))
		{
			logger->info("Skipping the local node");
			return;
		}

		if (std::find_if(m_remotes.begin(), m_remotes.end(), [&node_def](interfaces::node_shptr const& e) -> bool { return e->address() == node_def; }) == m_remotes.end())
		{
			sockpp::tcp_connector remote_socket({ node_def, OPTP_DEFAULT_PORT });
			if (!remote_socket)
			{
				logger->info("Cannot connect to host server\n\t{0}", remote_socket.last_error_str());
				return;
			}

			logger->info("Successfully connected to host {0}", node_def);
			interfaces::node_shptr rnode = std::make_shared<remote_node>(weak_from_this(), std::move(remote_socket));
			m_remotes.insert(rnode);
		}
	}

	void optp::disconnectFromNode(interfaces::node_wptr const& node_def)
	{
		if (const interfaces::node_shptr shnode = node_def.lock())
			disconnectFromNode(shnode->getDefinition());
	}

	void optp::disconnectFromNode(interfaces::node_def_wptr const& node_def)
	{
		std::string addr = "";
		if (const interfaces::node_def_shptr shnodedef = node_def.lock())
			addr = shnodedef->address();

		optp::node_list_t::const_iterator node_it = findNode(addr);
		if (node_it == m_remotes.end())
		{
			logger->warn("Node doesn't exist in the connected nodes list");
			return;
		}
		m_remotes.erase(node_it);
	}

	interfaces::node_wptr optp::getNode(interfaces::node_def_wptr const& node_def) const
	{
		if (const interfaces::node_def_shptr shnodedef = node_def.lock())
		{
			return getNodeByIpAddress(shnodedef->address());
		}
	}

	interfaces::node_wptr optp::getNodeByIpAddress(std::string const& ip_address) const
	{
		optp::node_list_t::const_iterator node_it = findNode(ip_address);
		if (node_it == m_remotes.end())
			return interfaces::node_wptr();

		return *node_it;
	}

	bool optp::startServer()
	{
		m_serverSocket = std::move(sockpp::tcp_acceptor(OPTP_DEFAULT_PORT, 10));
		logger->info("Local server started on {0}", static_cast<sockpp::inet_address>(m_serverSocket.address()).to_string());
		if (!m_serverSocket)
		{
			// TODO: print error log
			return false;
		}

		// Create a main server thread to listen for incoming connections.
		std::thread([&optp = *this, &remotes = m_remotes, &server = m_serverSocket, &maxConCount = cm_maxConnectionCount, &pnode = m_thisNode]() -> bool {
			while (1)
			{
				sockpp::inet_address peer_address;
				sockpp::tcp_socket peer_socket = server.accept(&peer_address);

				logger->info("Peer is connecting with address {0}", peer_address.to_string());
				if (!peer_socket)
				{
					logger->error("Peer socket connection problem\n\t{0}", peer_socket.last_error_str());
					continue;
				}

				if (maxConCount > 0)
				{
					if (remotes.size() == maxConCount)
					{
						// max connection count reached
						const std::string err_msg = "Maximum connection count reached.";
						logger->error(err_msg);
						peer_socket.write(err_msg);
						peer_socket.close();
					}
				}

				logger->info("Remote node was created for peer {0}", peer_address.to_string());
				interfaces::node_shptr rnode = std::make_shared<remote_node>(optp.weak_from_this(), std::move(peer_socket));
				remotes.insert(rnode);
			}
		}).detach();

		return true;
	}

	bool optp::connectToServer()
	{
		for (optp_config::node_def_t node : m_configuration.cluster_definition())
		{
			connectToNode(node);
		}

		return true;
	}

	optp::node_list_t::const_iterator optp::findNode(optp_config::node_def_t const& node_def) const
	{
		optp::node_list_t::const_iterator node_it = std::find_if(m_remotes.begin(), m_remotes.end(), [&node_def](interfaces::node_shptr const& e) -> bool {
			return e->address().find_first_of(node_def) != std::string::npos;
			});

		return node_it;
	}
}
