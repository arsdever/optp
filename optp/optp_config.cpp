#include "optp_config.h"

namespace optp
{
	optp_config::optp_config(std::string const& file_path)
	{
		m_content = nlohmann::json::parse(file_path);
	}

	optp_config optp_config::parse(std::string const& file_path)
	{
		return std::move(optp_config(file_path));
	}
}