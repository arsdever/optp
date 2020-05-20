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
#include "interpreter.h"

#include <spdlog/spdlog.h>
#include <thread>

struct simpleOpeartion : public optp::operation
{
};

int main(int argc, char** argv)
{
	optp::optp* protocol1 = new optp::optp("test_config.json");

	std::string command;

	optp::test::interpreter interpreter(std::cin);
	volatile bool finished = false;

	interpreter.registerCallback("send", [=]() {
		simpleOpeartion operation;
		protocol1->executeOperation(operation);
	});

	auto finisher = [&finished]() { finished = true; };

	interpreter.registerCallback("exit", finisher);
	interpreter.registerCallback("quit", finisher);
	interpreter.registerCallback("e", finisher);
	interpreter.registerCallback("q", finisher);

	interpreter.exec();

	while (!finished)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	delete protocol1;

	return 0;
}
