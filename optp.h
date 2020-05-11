#pragma once

#include <optp_lib.h>
#include <optp_config.h>

#include <string>

namespace optp
{
	class OPTP_LIB optp
	{
	public:
		optp(std::string const& config_file_path);

		bool start();

	private:
		optp_config m_configuration;
	};
}