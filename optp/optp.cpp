#include "optp.h"

namespace optp
{
	optp::optp(std::string const& config_file_path)
		: m_configuration(optp_config::parse(config_file_path))
	{}
}