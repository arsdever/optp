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

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>

static auto sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
static auto logger = std::make_shared<spdlog::logger>("optp_config", sink);

namespace optp
{
	optp_config::optp_config(std::string const& file_path)
	{
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
		std::ifstream istm(file_path);
		if (!istm)
		{
			logger->error("Could not open file {0}: {1}", file_path, strerror(errno));
			return;
		}

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
