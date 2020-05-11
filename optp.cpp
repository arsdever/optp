#include "optp.h"

#include <thread>

namespace optp
{
	optp::optp(std::string const& config_file_path)
		: m_configuration(optp_config::parse(config_file_path))
	{}

	bool optp::start()
	{
		std::vector<std::thread*> connection_checker_threads;
		volatile bool connection_established = false;
		for (optp_config::node_def_t const& node_def : m_configuration.cluster_definition())
		{
			std::thread* connection_checker_thread = new std::thread([&connection_established]() {

				});
			connection_checker_threads.push_back(connection_checker_thread);
		}

		return true;
	}
}