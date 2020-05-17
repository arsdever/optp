#pragma once

#include "fake_node.h"

namespace optp
{
	namespace test
	{
		std::string fake_node::address() const
		{
			return "fake_address";
		}

		interfaces::operation& fake_node::execute(interfaces::operation& operation)
		{
			return operation;
		}

		interfaces::operation& fake_node::handle(interfaces::operation& operation)
		{
			return operation;
		}

		std::string fake_node::uuid() const
		{
			return "fake_node_uuid";
		}
	}
}