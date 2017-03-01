#include "lrbProtoBuf.h"


using namespace lrb::ProtoBuf;

#ifdef LRB_APPSERVER
ReqVerifyData g_ReqVerifyData;
ReqRoleInfo g_ReqRoleInfo;
void *g_lrb_protobuf_ptrs[] = {&g_ReqVerifyData,&g_ReqRoleInfo,};
#else
AckVerifyData g_AckVerifyData;
AckRoleInfo g_AckRoleInfo;
void *s_lrb_protobuf_ptrs[] = {&g_AckVerifyData,&g_AckRoleInfo,};
#endif

short g_lrb_protobuf_confs[][5] = {
{0,0,4,0,0},
{0,0,4,0,0},
{24,0,4,0,0},
{24,0,4,0,0},
};
