/*
 * =====================================================================================
 *
 *		Filename:	test/test.cpp
 *		Author:		Arsen Gharagyozyan (arsdever), arsen.gharagyozyn.96@gmail.com
 *
 * =====================================================================================
 */

#include <optp.h>
#include <operation.h>
#include "fake_node.h"

struct simpleOpeartion : public optp::operation
{
};

int main(int argc, char** argv)
{
	optp::optp protocol1("test_config.json");

	while (1) {
		simpleOpeartion op;
		protocol1.executeOperation(op);
	}

	return 0;
}
