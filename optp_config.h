#pragma once

#include <nlohmann/json.hpp>

#include <optp_export.h>

namespace optp
{
	class OPTP_EXPORT optp_config
	{
	public:
		typedef std::string node_def_t;
		typedef std::vector<node_def_t> cluster_def_t;

	public:
		optp_config(std::string const& file_path);

		cluster_def_t const& cluster_definition() const;

		static optp_config parse(std::string const& file_path);

	private:
		cluster_def_t m_cluster_nodes;
		nlohmann::json m_content;
	};
}