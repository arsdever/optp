/*
 * =====================================================================================
 *
 *		Filename:	optp/node_def.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

namespace optp
{
	namespace interfaces
	{
		enum class node_group : unsigned int
		{
			GROUP_1 = 0x1 << 1,
			GROUP_2 = 0x1 << 2,
			GROUP_3 = 0x1 << 3,
			GROUP_4 = 0x1 << 4,
			GROUP_5 = 0x1 << 5,
			GROUP_6 = 0x1 << 6,
			GROUP_7 = 0x1 << 7,
			GROUP_8 = 0x1 << 8,
			GROUP_9 = 0x1 << 9,
			GROUP_10 = 0x1 << 10,
			ALL = 0xffffffff,
			ANY = ALL
		};

		class node_def
		{
		public:
			virtual ~node_def() = default;

			virtual std::string address() const = 0;
			virtual void setAddress(std::string const& address) = 0;
			virtual node_group group() const = 0;
			virtual void set_group(node_group) = 0;
		};
	}
}
