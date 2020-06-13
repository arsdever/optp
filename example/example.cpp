/*
 * =====================================================================================
 *
 *		Filename:	example/example.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include <optp.h>
#include <operation.h>
#include <uuid_provider.h>

#include "interpreter.h"

#include <spdlog/spdlog.h>
#include <thread>

class simple_operation : public optp::interfaces::operation
{
public:
	inline simple_operation() : m_uuid(std::move(optp::uuid_provider().provideRandomString())) {}
	inline void deserialize(std::string const& dataBuffer) override { m_uuid = dataBuffer; }
	inline std::string serialize() const override { return uuid(); }
	inline std::string uuid() const override { return m_uuid; }
	inline void setResult(optp::interfaces::operation_result_shptr result) override {}

private:
	std::string m_uuid;
};

int main(int argc, char** argv)
{
	optp::optp* protocol1 = new optp::optp("test_config.json");

	std::string command;

	optp::test::interpreter interpreter(std::cin);
	volatile bool finished = false;

	interpreter.registerCallback("send", [=](std::istream&) {
		optp::interfaces::operation_shptr operation = std::make_shared<simple_operation>();
		if(const optp::interfaces::node_shptr node = protocol1->thisNode().lock())
			node->execute(operation);
	});

	auto finisher = [&finished](std::istream&) { finished = true; };

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
