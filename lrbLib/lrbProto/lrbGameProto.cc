#include "lrbGameProto.h"



#ifdef LRB_GameProto_SERVER
ReqStreamData g_lrb_GameProto_ReqStreamData;
ReqVerifyData g_lrb_GameProto_ReqVerifyData;
ReqRoleInfo g_lrb_GameProto_ReqRoleInfo;
void *g_lrb_GameProto_ptrs[] = {
&g_lrb_GameProto_ReqStreamData,
&g_lrb_GameProto_ReqVerifyData,
&g_lrb_GameProto_ReqRoleInfo,
};
#else
AckStreamData g_lrb_GameProto_AckStreamData;
AckVerifyData g_lrb_GameProto_AckVerifyData;
AckRoleInfo g_lrb_GameProto_AckRoleInfo;
void *g_lrb_GameProto_ptrs[] = {
&g_lrb_GameProto_AckStreamData,
&g_lrb_GameProto_AckVerifyData,
&g_lrb_GameProto_AckRoleInfo,
};
#endif

short g_lrb_GameProto_confs[][5] = {
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{1*sizeof(std::string),0,4,0,0},
{1*sizeof(std::string),0,4,0,0},
};

int g_lrb_GameProto_protoNum = 6;


