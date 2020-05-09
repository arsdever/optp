#pragma once

#include <nlohmann/json.hpp>

namespace optp
{
	class optp_config
	{
	public:
		optp_config(std::string const& file_path);

		static optp_config parse(std::string const& file_path);

	private:
		nlohmann::json m_content;
	};
}