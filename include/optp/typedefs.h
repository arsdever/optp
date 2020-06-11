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
		class optp;
		class node;
		class node_def;
		class operation;
		class operation_result;
		class operation_handler;

		typedef std::shared_ptr<node> node_shptr;
		typedef std::weak_ptr<node> node_wptr;
		typedef node& node_ref;
		typedef node const& node_cref;

		typedef std::shared_ptr<node_def> node_def_shptr;
		typedef std::weak_ptr<node_def> node_def_wptr;
		typedef node_def& node_def_ref;
		typedef node_def const& node_def_cref;

		typedef std::shared_ptr<operation> operation_shptr;
		typedef std::weak_ptr<operation> operation_wptr;
		typedef operation& operation_ref;
		typedef operation const& operation_cref;

		typedef std::shared_ptr<operation_result> operation_result_shptr;
		typedef std::weak_ptr<operation_result> operation_result_wptr;
		typedef operation_result& operation_result_ref;
		typedef operation_result const& operation_result_cref;

		typedef std::shared_ptr<optp> optp_shptr;
		typedef std::weak_ptr<optp> optp_wptr;
		typedef optp& optp_ref;
		typedef optp const& optp_cref;

		typedef std::shared_ptr<operation_handler> operation_handler_shptr;
		typedef std::weak_ptr<operation_handler> operation_handler_wptr;
		typedef operation_handler& operation_handler_ref;
		typedef operation_handler const& operation_handler_cref;
	}
}
