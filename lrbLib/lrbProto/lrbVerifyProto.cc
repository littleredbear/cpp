#include "lrbVerifyProto.h"



#ifdef LRB_VerifyProto_SERVER
ReqStreamData g_lrb_VerifyProto_ReqStreamData;
ReqVerifyData g_lrb_VerifyProto_ReqVerifyData;
void *g_lrb_VerifyProto_ptrs[] = {
&g_lrb_VerifyProto_ReqStreamData,
&g_lrb_VerifyProto_ReqVerifyData,
};
#else
AckStreamData g_lrb_VerifyProto_AckStreamData;
AckVerifyData g_lrb_VerifyProto_AckVerifyData;
void *g_lrb_VerifyProto_ptrs[] = {
&g_lrb_VerifyProto_AckStreamData,
&g_lrb_VerifyProto_AckVerifyData,
};
#endif

short g_lrb_VerifyProto_confs[][5] = {
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
};

int g_lrb_VerifyProto_protoNum = 4;


