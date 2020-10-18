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

#include <unordered_set>

namespace optp
{
	class connector;

	class OPTP_EXPORT optp : public interfaces::optp
	{
	private:
		typedef std::unordered_set<interfaces::node_shptr> node_list_t;

	public:
		optp(std::string const& config_file_path, interfaces::node_shptr node);
		optp(std::string const& config_file_path);
		~optp();

		void startup();

		interfaces::node_wptr thisNode() const;
		void initializeSupportedOperations() const;

		interfaces::operation_shptr execute(interfaces::operation_shptr operation);
		interfaces::operation_shptr handle(interfaces::operation_shptr operation);

		void forEachRemote(std::function<void(interfaces::node_wptr)> function, bool skip_valid_check = true);
		void connectToNode(std::string const& ip_address) override;
		void disconnectFromNode(interfaces::node_def_wptr const& node_def) override;
		void disconnectFromNode(interfaces::node_wptr const& node_def) override;
		interfaces::node_wptr getNode(interfaces::node_def_wptr const& node_def) const override;
		interfaces::node_wptr getNodeByIpAddress(std::string const& ip_address) const override;

	private:
		bool startServer();
		bool connectToServer();

		node_list_t::const_iterator findNode(optp_config::node_def_t const& node_def) const;

	private:
		optp_config m_configuration;
		interfaces::node_shptr m_thisNode;
		node_list_t m_remotes;
		std::unique_ptr<connector> m_connector;
	};
}
