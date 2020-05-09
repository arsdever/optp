#pragma once

#include <string>

#include <optp_config.h>

namespace optp
{
	class optp
	{
	public:
		optp(std::string const& config_file_path);

	private:
		optp_config m_configuration;
	};
}