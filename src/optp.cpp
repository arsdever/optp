/*
 * =====================================================================================
 *
 *		Filename:	optp.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "optp.h"

#include "operation.h"
#include "operation_result.h"
#include "remote_node.h"
#include "real_node.h"
#include "network_interfaces.h"
#include "node_def.h"
#include "connector.h"
#include <optp/object_metatypes.h>

#include "operations/node_uuid_getter_op.h"

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("optp", sink);

extern const int OPTP_DEFAULT_PORT;

namespace optp
{
	namespace
	{
		/**
		 * @brief Resolve the plain address string into parts.
		 *
		 * Break-down the address and split it into two parts:
		 *  - IP address
		 *  - Port number
		 * @param address the address to be resolved
		 * @return a touple of resolved values
		 */
		std::tuple<std::string, std::string> breakdown_address(std::string address)
		{
			size_t colon_position = address.find_first_of(':');
			std::string port_string = address.substr(colon_position + 1);
			std::string ip_string = address.substr(0, colon_position);
			return std::make_tuple(ip_string, port_string);
		}
	}

	optp::optp(std::string const& config_file_path, interfaces::node_shptr node)
		: m_configuration(optp_config::parse(config_file_path))
		, m_thisNode(node)
	{
		object_metatypes::object_ctor_mapping_generator::generate();
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
		initializeSupportedOperations();
		startServer();
		connectToServer();
	}

	optp::optp(std::string const& config_file_path)
		: m_configuration(optp_config::parse(config_file_path))
	{
		object_metatypes::object_ctor_mapping_generator::generate();
		m_thisNode = std::move(std::make_shared<real_node>(weak_from_this()));
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
		initializeSupportedOperations();
		startServer();
		connectToServer();
	}

	optp::~optp()
	{
		// Disconnect from all connections
		// Cleanup
	}

	void optp::initializeSupportedOperations() const
	{
		m_thisNode->registerAnOperationType<operations::node_uuid_getter_operation>();
	}

	interfaces::node_wptr optp::thisNode() const
	{
		return m_thisNode;
	}

	void optp::forEachRemote(std::function<void(interfaces::node_wptr)> function, bool skip_valid_check)
	{
		for (interfaces::node_shptr node : m_remotes)
			function(node);
	}

	interfaces::operation_shptr optp::execute(interfaces::operation_shptr operation)
	{
		// TODO: Make this to work in parallel for all nodes
		m_thisNode->handle(operation);
		{
			for (interfaces::node_shptr remote : m_remotes)
			{
				remote->execute(operation);
			}
		}
		return operation;
	}

	interfaces::operation_result_shptr optp::handle(interfaces::operation_shptr operation)
	{
		return m_thisNode->handle(operation);
	}

	void optp::connectToNode(std::string const& node_address)
	{
		/*if (network_interfaces::global().is_local(node_address))
		{
			logger->info("Skipping the local node");
			return;
		}*/

		//bool exists = std::find_if(m_remotes.begin(), m_remotes.end(), [&node_address](interfaces::node_shptr const& e) -> bool { return e->address() == node_address; }) == m_remotes.end();
	
		//if (exists)
		//{
			/*sockpp::tcp_connector remote_socket({ node_address, (unsigned short)OPTP_DEFAULT_PORT });
			if (!remote_socket)
			{
				logger->info("Cannot connect to host server\n\t{0}", remote_socket.last_error_str());
				return;
			}

			logger->info("Successfully connected to host {0}", node_address);

			if (const interfaces::node_shptr current_node = thisNode().lock())
			{
				if (const interfaces::serializable_shptr current_node_def = std::dynamic_pointer_cast<interfaces::serializable>(current_node->getDefinition().lock()))
				{
					std::stringstream strm;
					current_node_def->serialize(strm);
					std::string def_message = strm.str().substr(5);
					logger->info("Sending the node information\n\t{0}", def_message);
					remote_socket.write(def_message);
				}
			}

			char buffer[1024];
			int read_bytes;

			interfaces::node_def_shptr def = std::move(std::make_shared<node_def>(node_address));
			while ((read_bytes = remote_socket.read(buffer, sizeof(buffer))) > 0)
			{
				std::string message(buffer, read_bytes);
				std::stringstream strm(message);
				strm.ignore(5);
				logger->info("node_def received from node with address {0}\n\t{1}", remote_socket.address().to_string(), message);
				interfaces::deserializable_shptr ptr = std::dynamic_pointer_cast<interfaces::deserializable>(def);
				ptr->deserialize(strm);
				break;
			}

			std::shared_ptr<remote_node> rnode = std::make_shared<remote_node>(weak_from_this(), std::move(remote_socket), std::move(def));
				m_remotes.insert(rnode);*/
		//}
		interfaces::node_shptr node = m_connector->connect_to(node_address/*, [&remotes = this->m_remotes](interfaces::node_shptr node) {
				remotes.insert(node);
			}*/);
		m_remotes.insert(node);
		rnode_shptr rnode = std::dynamic_pointer_cast<remote_node>(node);
		if (rnode)
		{
			rnode->set_event_handlers(remote_node::event_handler_mapping{}.on_disconnect([&remotes = m_remotes](interfaces::node_wptr wnode) {
				if (interfaces::node_shptr shnode = wnode.lock())
				{
					std::string uuid = std::dynamic_pointer_cast<interfaces::object>(shnode->getDefinition().lock())->uuid();
					remotes.erase(shnode);
					logger->info("Node {} was removed from remotes list", uuid);
				}
				}));
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

		return interfaces::node_wptr();
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
		m_connector = std::make_unique<connector>();
		m_connector->start();
		m_connector->register_on_connection([&remotes = this->m_remotes](interfaces::node_shptr node) {
			remotes.insert(node);
			});
		//	[&](sockpp::tcp_socket&& peer_socket, sockpp::inet_address&& peer_address)
		//	{
		//		if (cm_maxConnectionCount > 0)
		//		{
		//			if (m_remotes.size() == cm_maxConnectionCount)
		//			{
		//				// max connection count reached
		//				const std::string err_msg = "Maximum connection count reached.";
		//				logger->error(err_msg);
		//				peer_socket.write(err_msg);
		//				peer_socket.close();
		//			}
		//		}

		//		char buffer[1024];
		//		size_t read_bytes;

		//		std::string ip_address;
		//		std::tie(ip_address, std::ignore) = breakdown_address(peer_address.to_string());

		//		interfaces::node_def_shptr def = std::move(std::make_shared<node_def>(ip_address));
		//		while ((read_bytes = peer_socket.read(buffer, sizeof(buffer))) > 0)
		//		{
		//			std::string message(buffer, read_bytes);
		//			std::stringstream strm(message);
		//			strm.ignore(5);
		//			//logger->info("node_def received from node with address {0}\n\t{1}", peer_socket.address().to_string(), message);
		//			interfaces::deserializable_shptr ptr = std::dynamic_pointer_cast<interfaces::deserializable>(def);
		//			ptr->deserialize(strm);
		//			break;
		//		}

		//		if (const interfaces::node_shptr local_node = thisNode().lock())
		//		{
		//			if (const interfaces::serializable_shptr local_node_def = std::dynamic_pointer_cast<interfaces::serializable>(local_node->getDefinition().lock()))
		//			{
		//				std::stringstream strm;
		//				local_node_def->serialize(strm);
		//				std::string def_message = strm.str();
		//				logger->info("Sending the node information\n\t{0}", def_message);
		//				peer_socket.write(def_message);
		//			}
		//		}

		//		logger->info("Remote node was created for peer {0}", peer_address.to_string());
		//		std::shared_ptr<remote_node> rnode = std::make_shared<remote_node>(weak_from_this(), std::move(peer_socket), std::move(def));
		//		m_remotes.insert(rnode);
		//	}
		//);
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
