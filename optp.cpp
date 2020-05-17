/*
 * =====================================================================================
 *
 *		Filename:	optp.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include "optp.h"

#include "remote_node.h"
#include "real_node.h"

#include <sockpp/tcp_acceptor.h>
#include <sockpp/tcp_connector.h>
#include <thread>

static sockpp::socket_initializer sockppInit;

namespace optp
{
	optp::optp(std::string const& config_file_path, interfaces::node_shptr node)
		: cm_maxConnectionCount(0)
		, m_configuration(optp_config::parse(config_file_path))
		, m_thisNode(node)
	{
		startServer();
	}

	optp::optp(std::string const& config_file_path)
		: optp(config_file_path, std::move(std::make_shared<real_node>()))
	{}

	optp::~optp()
	{}

	void optp::executeOperation(interfaces::operation_ref operation)
	{
		m_thisNode->execute(operation);
	}

	bool optp::startServer()
	{
		m_serverSocket = std::move(sockpp::tcp_acceptor(33000, 10));
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

				if (!peer_socket)
				{
					// TODO: print error log
					continue;
				}

				if (maxConCount > 0)
				{
					if (remotes.size() == maxConCount)
					{
						// max connection count reached
						// TODO: print connection drop log
						peer_socket.write("Maximum connection count reached.");
						peer_socket.close();
					}
				}

				interfaces::node_shptr rnode = std::make_shared<remote_node>(std::move(peer_socket));
				std::static_pointer_cast<real_node>(pnode)->registerRemoteNode(rnode);
				remotes.insert(rnode);
			}
		}).detach();

		return true;
	}

	bool optp::connectToServer()
	{
		return true;
	}
}
