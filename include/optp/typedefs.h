/*
 * =====================================================================================
 *
 *		Filename:	optp/typedef.h
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#pragma once
#include <memory>

namespace optp
{
	namespace interfaces
	{
		class node;
		class operation;

		typedef std::shared_ptr<node> node_shptr;
		typedef std::weak_ptr<node> node_wptr;
		typedef node& node_ref;
		typedef node const& node_cref;

		typedef std::shared_ptr<operation> operation_shptr;
		typedef std::weak_ptr<operation> operation_wptr;
		typedef operation& operation_ref;
		typedef operation const& operation_cref;
	}
}
