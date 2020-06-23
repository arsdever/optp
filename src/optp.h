/*
 * =====================================================================================
 *
 *		Filename:	optp.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include <optp_lib.h>
#include <optp_config.h>
#include <optp_export.h>

#include <optp/typedefs.h>
#include <optp/optp.h>
#include <optp/node.h>

#include <string>
#include <memory>
#include <unordered_set>
#include <sockpp/acceptor.h>


namespace optp
{
	class OPTP_EXPORT optp : interfaces::optp
	{
	private:
		typedef std::unordered_set<interfaces::node_shptr> node_list_t;

	public:
		optp(std::string const& config_file_path, interfaces::node_shptr node);
		optp(std::string const& config_file_path);
		~optp();

		interfaces::node_wptr thisNode() const;

		interfaces::operation_shptr execute(interfaces::operation_shptr operation);
		interfaces::operation_shptr handle(interfaces::operation_shptr operation);

		void connectToNode(optp_config::node_def_t const& node_def);
		void disconnectFromNode(optp_config::node_def_t const& node_def);
		interfaces::node_wptr getNode(optp_config::node_def_t const& node_def);

	private:
		bool startServer();
		bool connectToServer();
		node_list_t::iterator findNode(optp_config::node_def_t const& node_def);

	private:
		const int cm_maxConnectionCount;
		optp_config m_configuration;
		interfaces::node_shptr m_thisNode;
		node_list_t m_remotes;
		sockpp::acceptor m_serverSocket;
	};
}
