#include "lrbGameProto.h"


using namespace lrb::GameProto;

#ifdef LRB_GameProto_SERVER
ReqStreamData g_lrb_GameProto_ReqStreamData;
ReqAckFuncType g_lrb_GameProto_ReqAckFuncType;
ReqVerifyData g_lrb_GameProto_ReqVerifyData;
ReqErrorCode g_lrb_GameProto_ReqErrorCode;
ReqRoleId g_lrb_GameProto_ReqRoleId;
ReqRoleName g_lrb_GameProto_ReqRoleName;
ReqRolePos g_lrb_GameProto_ReqRolePos;
ReqLogin g_lrb_GameProto_ReqLogin;
ReqModId g_lrb_GameProto_ReqModId;
ReqFightRoleData g_lrb_GameProto_ReqFightRoleData;
ReqFightAttack g_lrb_GameProto_ReqFightAttack;
ReqFightDefend g_lrb_GameProto_ReqFightDefend;
ReqFightBuff g_lrb_GameProto_ReqFightBuff;
ReqFightRound g_lrb_GameProto_ReqFightRound;
ReqFight g_lrb_GameProto_ReqFight;
ReqUseItem g_lrb_GameProto_ReqUseItem;
ReqChatInfo g_lrb_GameProto_ReqChatInfo;
void *g_lrb_GameProto_ptrs[] = {
&g_lrb_GameProto_ReqStreamData,
&g_lrb_GameProto_ReqAckFuncType,
&g_lrb_GameProto_ReqVerifyData,
&g_lrb_GameProto_ReqErrorCode,
&g_lrb_GameProto_ReqRoleId,
&g_lrb_GameProto_ReqRoleName,
&g_lrb_GameProto_ReqRolePos,
&g_lrb_GameProto_ReqLogin,
&g_lrb_GameProto_ReqModId,
&g_lrb_GameProto_ReqFightRoleData,
&g_lrb_GameProto_ReqFightAttack,
&g_lrb_GameProto_ReqFightDefend,
&g_lrb_GameProto_ReqFightBuff,
&g_lrb_GameProto_ReqFightRound,
&g_lrb_GameProto_ReqFight,
&g_lrb_GameProto_ReqUseItem,
&g_lrb_GameProto_ReqChatInfo,
};

#else
AckStreamData g_lrb_GameProto_AckStreamData;
AckAckFuncType g_lrb_GameProto_AckAckFuncType;
AckVerifyData g_lrb_GameProto_AckVerifyData;
AckErrorCode g_lrb_GameProto_AckErrorCode;
AckRoleId g_lrb_GameProto_AckRoleId;
AckRoleName g_lrb_GameProto_AckRoleName;
AckRolePos g_lrb_GameProto_AckRolePos;
AckLogin g_lrb_GameProto_AckLogin;
AckModId g_lrb_GameProto_AckModId;
AckFightRoleData g_lrb_GameProto_AckFightRoleData;
AckFightAttack g_lrb_GameProto_AckFightAttack;
AckFightDefend g_lrb_GameProto_AckFightDefend;
AckFightBuff g_lrb_GameProto_AckFightBuff;
AckFightRound g_lrb_GameProto_AckFightRound;
AckFight g_lrb_GameProto_AckFight;
AckUseItem g_lrb_GameProto_AckUseItem;
AckChatInfo g_lrb_GameProto_AckChatInfo;
void *g_lrb_GameProto_ptrs[] = {
&g_lrb_GameProto_AckStreamData,
&g_lrb_GameProto_AckAckFuncType,
&g_lrb_GameProto_AckVerifyData,
&g_lrb_GameProto_AckErrorCode,
&g_lrb_GameProto_AckRoleId,
&g_lrb_GameProto_AckRoleName,
&g_lrb_GameProto_AckRolePos,
&g_lrb_GameProto_AckLogin,
&g_lrb_GameProto_AckModId,
&g_lrb_GameProto_AckFightRoleData,
&g_lrb_GameProto_AckFightAttack,
&g_lrb_GameProto_AckFightDefend,
&g_lrb_GameProto_AckFightBuff,
&g_lrb_GameProto_AckFightRound,
&g_lrb_GameProto_AckFight,
&g_lrb_GameProto_AckUseItem,
&g_lrb_GameProto_AckChatInfo,
};

#endif

short g_lrb_GameProto_confs[][5] = {
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{1*sizeof(std::string),0,0,0,0},
{1*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,8,0,0},
{0*sizeof(std::string),0,8,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,8,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,12,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,12,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,8,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,16,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,12,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,4,0,1},
{0*sizeof(std::string),0,4,0,1},
};

namespace lrb {

namespace GameProto {

void *getUnpackDest(int protoId)
{
	if (protoId < 0 || protoId >= 34)
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
	if (protoId < 0 || protoId >= 34)
		return NULL;

	return g_lrb_GameProto_confs[protoId];
}

}
}

