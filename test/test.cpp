/*
 * =====================================================================================
 *
 *		Filename:	test/test.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include <optp.h>
#include <operation.h>
#include "fake_node.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

struct simpleOpeartion : public optp::operation
{
};

int main(int argc, char** argv)
{
	spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
	auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
	spdlog::set_default_logger(file_logger);

	optp::optp protocol1("test_config.json");

	while (1) {
		simpleOpeartion op;
		protocol1.executeOperation(op);
	}

	return 0;
}
