#include "lrbNetData.h"
#include <stdlib.h>


using namespace lrb::NetData;

ReqVerifyData g_VerifyData;

const static void *s_ptrs[] = {&g_VerifyData,};

const static short s_confs[][5] = {
	{0,0,4,0,0,},
};


int archiveData(const void *src, int uuid, void **res)
{
	
	return -1;
}

void parseData(void *src)
{

}
