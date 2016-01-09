#include "../lrb.h"

int main (int argc, char **argv)
{
	char log[] = "test log";
	lrb::LrbLOG(log, strlen(log));
	char err[] = "test error";
	lrb::LrbERROR(err, strlen(err));
}
