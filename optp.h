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
#include <interface/typedefs.h>

#include <string>
#include <memory>
#include <unordered_set>
#include <sockpp/acceptor.h>
#include <interface/node.h>

#include <optp_export.h>

namespace optp
{
	class OPTP_EXPORT optp
	{
	public:
		optp(std::string const& config_file_path, interfaces::node_shptr node);
		optp(std::string const& config_file_path);
		~optp();

		void executeOperation(interfaces::operation_ref operation);

	private:
		bool startServer();
		bool connectToServer();

	private:
		const int cm_maxConnectionCount;
		optp_config m_configuration;
		interfaces::node_shptr m_thisNode;
		std::unordered_set<interfaces::node_shptr> m_remotes;
		sockpp::acceptor m_serverSocket;
	};
}
