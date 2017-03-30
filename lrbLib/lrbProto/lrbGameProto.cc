#include "lrbGameProto.h"


using namespace lrb::GameProto;

#ifdef LRB_GameProto_SERVER
ReqStreamData g_lrb_GameProto_ReqStreamData;
ReqVerifyData g_lrb_GameProto_ReqVerifyData;
ReqErrorCode g_lrb_GameProto_ReqErrorCode;
ReqRoleInfo g_lrb_GameProto_ReqRoleInfo;
ReqRoleName g_lrb_GameProto_ReqRoleName;
ReqRolePos g_lrb_GameProto_ReqRolePos;
ReqUseItem g_lrb_GameProto_ReqUseItem;
void *g_lrb_GameProto_ptrs[] = {
&g_lrb_GameProto_ReqStreamData,
&g_lrb_GameProto_ReqVerifyData,
&g_lrb_GameProto_ReqErrorCode,
&g_lrb_GameProto_ReqRoleInfo,
&g_lrb_GameProto_ReqRoleName,
&g_lrb_GameProto_ReqRolePos,
&g_lrb_GameProto_ReqUseItem,
};

#else
AckStreamData g_lrb_GameProto_AckStreamData;
AckVerifyData g_lrb_GameProto_AckVerifyData;
AckErrorCode g_lrb_GameProto_AckErrorCode;
AckRoleInfo g_lrb_GameProto_AckRoleInfo;
AckRoleName g_lrb_GameProto_AckRoleName;
AckRolePos g_lrb_GameProto_AckRolePos;
AckUseItem g_lrb_GameProto_AckUseItem;
void *g_lrb_GameProto_ptrs[] = {
&g_lrb_GameProto_AckStreamData,
&g_lrb_GameProto_AckVerifyData,
&g_lrb_GameProto_AckErrorCode,
&g_lrb_GameProto_AckRoleInfo,
&g_lrb_GameProto_AckRoleName,
&g_lrb_GameProto_AckRolePos,
&g_lrb_GameProto_AckUseItem,
};

#endif

short g_lrb_GameProto_confs[][5] = {
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{1*sizeof(std::string),0,0,0,0},
{1*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,8,0,0},
{0*sizeof(std::string),0,8,0,0},
{0*sizeof(std::string),0,12,0,0},
{0*sizeof(std::string),0,0,0,0},
};

namespace lrb {

namespace GameProto {

void *getUnpackDest(int protoId)
{
	if (protoId < 0 || protoId >= 14)
		return NULL;

#ifdef LRB_GameProto_SERVER
	if (protoId & 1)
#else
	if (!(protoId & 1))
#endif
		return NULL;

	return g_lrb_GameProto_ptrs[protoId >> 1];
}

short *getProtoConfs(int protoId)
{
	if (protoId < 0 || protoId >= 14)
		return NULL;

	return g_lrb_GameProto_confs[protoId];
}

}
}

