/*
 * =====================================================================================
 *
 *		Filename:	example/ping_operation.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */
#pragma once
#include <operation.h>

namespace optp
{
	namespace test
	{
		enum class operation_types
		{
			PingOperation
		};

		class ping_operation : public operation
		{
		public:
			ping_operation();
			int type() const override;
			std::string serialize() const override;
			void deserialize(std::string const& data) override;
			void setResult(interfaces::operation_result_shptr result) override;
		};
	}
}