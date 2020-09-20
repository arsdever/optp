#pragma once

#include "node"
;#include "node_def"
;#include "object"
;#include "operation"
;#include "operation_result"


namespace optp
{
	class node;
	;class node_def;
	;class object;
	;class operation;
	;class operation_result;
	

	class object_factory
	{
		template <typename ARGS...>
		static object_shptr construct(object_metatype type, ARGS args)
		{
			switch(type)
			{
			case NODE: return std::make_shared(args);;case NODE_DEF: return std::make_shared(args);;case OBJECT: return std::make_shared(args);;case OPERATION: return std::make_shared(args);;case OPERATION_RESULT: return std::make_shared(args);
			default: return nullptr;
			}
		}
	}
}
