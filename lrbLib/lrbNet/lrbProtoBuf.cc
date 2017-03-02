#include "lrbProtoBuf.h"


using namespace lrb::ProtoBuf;

#ifdef LRB_APPSERVER
ReqVerifyData g_lrb_protobuf_ReqVerifyData;
ReqRoleInfo g_lrb_protobuf_ReqRoleInfo;
void *g_lrb_protobuf_ptrs[] = {
&g_lrb_protobuf_ReqVerifyData,
&g_lrb_protobuf_ReqRoleInfo,
};
#else
AckVerifyData g_lrb_protobuf_AckVerifyData;
AckRoleInfo g_lrb_protobuf_AckRoleInfo;
void *g_lrb_protobuf_ptrs[] = {
&g_lrb_protobuf_AckVerifyData,
&g_lrb_protobuf_AckRoleInfo,
};
#endif

short g_lrb_protobuf_confs[][5] = {
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{1*sizeof(std::string),0,4,0,0},
{1*sizeof(std::string),0,4,0,0},
};
