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
		class node_def
		{
		public:
			virtual ~node_def() = default;

			virtual std::string address() const = 0;
			virtual void setAddress(std::string const& address) = 0;
		};
	}
}
