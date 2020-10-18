/*
 * =====================================================================================
 *
 *		Filename:	optp/optp.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once

#include <optp/typedefs.h>
#include <string.h>

namespace optp
{
	namespace interfaces
	{
		class optp : public std::enable_shared_from_this<optp>
		{
		public:
			virtual ~optp() = default;

			virtual node_wptr thisNode() const = 0;
			virtual operation_shptr execute(operation_shptr operation) = 0;
			virtual operation_result_shptr handle(operation_shptr operation) = 0;
			virtual void connectToNode(std::string const& ip_address) = 0;
			virtual void disconnectFromNode(node_def_wptr const& node_def) = 0;
			virtual void disconnectFromNode(node_wptr const& node_def) = 0;
			virtual node_wptr getNode(node_def_wptr const& node_def) const = 0;
			virtual node_wptr getNodeByIpAddress(std::string const& ip_address) const = 0;
		};
	}
}