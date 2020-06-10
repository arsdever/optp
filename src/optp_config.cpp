/*
 * =====================================================================================
 *
 *		Filename:	optp_config.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include "optp_config.h"
#include <fstream>

namespace optp
{
	optp_config::optp_config(std::string const& file_path)
	{
		std::ifstream istm(file_path);
		if(!istm)
			return;
		m_content = nlohmann::json::parse(istm);
		for (std::string const& node_ip : m_content["cluster"])
			m_cluster_nodes.push_back(node_ip);
	}

	optp_config::cluster_def_t const& optp_config::cluster_definition() const 
	{ 
		return m_cluster_nodes;
	}

	optp_config optp_config::parse(std::string const& file_path)
	{
		return std::move(optp_config(file_path));
	}
}
