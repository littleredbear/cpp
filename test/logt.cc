#include "lrb.h"
#include "lrbLog.h"
#include "lrbException.h"
#include <iostream>

using namespace lrb;
int main (int argc, char **argv)
{
	char log[] = "test log";
	LrbLOG(log, strlen(log));
	char err[] = "test error";
	LrbERROR(err, strlen(err));

	try {
		throw lrbLogException("test",1);
	} catch (lrbLogException &e) {
		std::cout << e.what() << std::endl;	
	}
}
