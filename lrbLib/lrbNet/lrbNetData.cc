#include "lrbNetData.h"
#include <stdlib.h>


#if LRB_APPTYPE == 0


#elif LRB_APPTYPE == 1


#else


#endif

ReqVerifyData g_ReqVerifyData;

const static void *s_ptrs[] = {&g_ReqVerifyData,};

static std::function<void()> s_reqFuncs[1];

#else

AckVerifyData g_AckVerifyData;

const static void *s_ptrs[] = {&g_AckVerifyData,};

#endif

const static short s_confs[][5] = {
	{0,0,4,0,0,},
	{0,0,4,0,0,},
};


namespace lrb {

namespace NetData {

int packData(const void *src, void **res)
{
	
	return -1;
}

void unpackData(void *src)
{
	int uuid;
	int idx = uuid >> 1;
	
}

#if LRB_APPTYPE == 0
void bindReqFunc(int uuid, const std::function<void()> &func)
{
	s_reqFuncs[uuid >> 1] = func;
}
#endif	

}

}
