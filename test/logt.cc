#include "lrb.h"
#include "lrbLog.h"

using namespace lrb;
int main (int argc, char **argv)
{
	char log[] = "test log";
	LrbLOG(log, strlen(log));
	char err[] = "test error";
	LrbERROR(err, strlen(err));
}
