/*
 * =====================================================================================
 *
 *		Filename:	example/fake_node.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp/node.h>

namespace optp
{
	namespace test
	{
		class fake_node : public interfaces::node
		{
		public:
			// interfaces::node started
			std::string address() const override; // TODO: Should be changed to address type
			interfaces::operation_ref execute(interfaces::operation_ref operation) override;
			interfaces::operation_ref handle(interfaces::operation_ref operation) override;
			std::string uuid() const override;
			// interfaces::node finished
		};
	}
}