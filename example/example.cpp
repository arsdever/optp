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
#include <network_interfaces.h>

#include "interpreter.h"
#include "ping_operation.h"

#include <spdlog/spdlog.h>

#include <thread>
#include <fstream>
#include <iostream>
#include <node_def.h>
#include <operations/node_uuid_getter_op.h>

int main(int argc, char** argv)
{
	std::shared_ptr<optp::optp> protocol;

	optp::test::interpreter interpreter(std::cin);
	volatile bool finished = false;

	interpreter.registerCallback("send", [&protocol](std::istream&) {
		optp::interfaces::operation_shptr operation = std::make_shared<optp::test::ping_operation>();
		optp::interfaces::node_wptr wnode = protocol->thisNode();
		if(const optp::interfaces::node_shptr node = wnode.lock())
			protocol->execute(operation);
	});

	auto finisher = [&finished](std::istream&) { finished = true; };

	interpreter.registerCallback("exit", finisher);
	interpreter.registerCallback("quit", finisher);
	interpreter.registerCallback("e", finisher);
	interpreter.registerCallback("q", finisher);
	interpreter.registerCallback("load", [&protocol](std::istream& stream) {
		std::string path;
		stream >> path;
		std::ifstream config_file(path);
		if (config_file)
		{
			std::string content = std::string(std::istreambuf_iterator<char>(config_file), std::istreambuf_iterator<char>());
			std::cout << "Using configuration\n|== Config begin ==|\n" << content << "\n|== Config end ==|" << std::endl;
		}
		protocol = std::make_unique<optp::optp>(path);
		protocol->startup();
		});
	interpreter.registerCallback("connect", [&protocol](std::istream& stream) {
		std::string address;
		stream >> address;
		if (protocol)
			protocol->connectToNode(address);
		});
	interpreter.registerCallback("disconnect", [&protocol](std::istream& stream) {
		std::string address;
		stream >> address;
		if (protocol)
			protocol->disconnectFromNode(protocol->getNodeByIpAddress(address));
		});
	interpreter.registerCallback("ip", [&protocol](std::istream& stream) {
		std::cout << "Local ip addresses are" << std::endl;
		std::vector<std::string> local_ip_addresses = optp::network_interfaces::global().local_addresses_string();
		for (std::string addr : local_ip_addresses)
		{
			std::cout << '\t' << addr << std::endl;
		}
		std::cout << std::endl;
		});
	interpreter.registerCallback("thisnode", [&protocol](std::istream& stream) {
		if (optp::interfaces::node_shptr nodesh = protocol->thisNode().lock())
		{
			if (optp::interfaces::object_shptr def = std::dynamic_pointer_cast<optp::interfaces::object>(nodesh->getDefinition().lock()))
			{
				std::cout << def->uuid() << std::endl;
			}
		}
		});
	interpreter.registerCallback("remotes", [&protocol](std::istream& stream) {
		protocol->forEachRemote([](optp::interfaces::node_wptr node) {
			if (optp::interfaces::node_shptr shnode = node.lock())
				if (optp::interfaces::object_shptr nodedef = std::dynamic_pointer_cast<optp::interfaces::object>(shnode->getDefinition().lock()))
					std::cout << nodedef->uuid() << " " << shnode->address() << std::endl;
			});
		});
	interpreter.registerCallback("getUuid", [&protocol](std::istream& stream) {
			optp::interfaces::operation_shptr operation = std::make_shared<optp::operations::node_uuid_getter_operation>();
			optp::interfaces::node_wptr wnode = protocol->thisNode();
			if (const optp::interfaces::node_shptr node = wnode.lock())
				protocol->execute(operation);

			std::list<optp::interfaces::operation_result_wptr> results = operation->getResults();
			std::cout << "=== The results are ===" << std::endl;
			for (optp::interfaces::operation_result_wptr wres : results)
			{
				if (const optp::operation_result_shptr res = std::dynamic_pointer_cast<optp::operation_result>(wres.lock()))
				{
					std::cout << res->data() << std::endl;
				}
			}
		});
	interpreter.exec();

	while (!finished)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}
