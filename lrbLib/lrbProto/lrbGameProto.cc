#include "lrbGameProto.h"


using namespace lrb::GameProto;

#ifdef LRB_GameProto_SERVER
ReqStreamData g_lrb_GameProto_ReqStreamData;
ReqVerifyData g_lrb_GameProto_ReqVerifyData;
ReqRoleInfo g_lrb_GameProto_ReqRoleInfo;
void *g_lrb_GameProto_ptrs[] = {
&g_lrb_GameProto_ReqStreamData,
&g_lrb_GameProto_ReqVerifyData,
&g_lrb_GameProto_ReqRoleInfo,
};
std::function<void(lrb::NetWork::DataPacker *)> g_lrb_GameProto_reqFuncs[6];
#else
AckStreamData g_lrb_GameProto_AckStreamData;
AckVerifyData g_lrb_GameProto_AckVerifyData;
AckRoleInfo g_lrb_GameProto_AckRoleInfo;
void *g_lrb_GameProto_ptrs[] = {
&g_lrb_GameProto_AckStreamData,
&g_lrb_GameProto_AckVerifyData,
&g_lrb_GameProto_AckRoleInfo,
};
std::function<void()> g_lrb_GameProto_ackFuncs[(int)AckFuncType::AFT_TOP];
#endif

short g_lrb_GameProto_confs[][5] = {
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{1*sizeof(std::string),0,4,0,0},
{1*sizeof(std::string),0,4,0,0},
};

namespace lrb {

namespace GameProto {

void *getUnpackDest(int protoId)
{
	if (protoId < 0 || protoId >= 6)
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
	if (protoId < 0 || protoId >= 6)
		return NULL;

	return g_lrb_GameProto_confs[protoId];
}

void execReqFunc(int protoId, lrb::NetWork::DataPacker *packer)
{
#ifdef LRB_GameProto_SERVER
	if (protoId < 0 || protoId >= 6 || (protoId & 1))
		return;

	g_lrb_GameProto_reqFuncs[protoId >> 1](packer);
#endif
}

void execAckFunc()
{
#ifndef LRB_GameProto_SERVER
	g_lrb_GameProto_ackFuncs[g_lrb_GameProto_AckVerifyData.verify]();
#endif
}

void bindReqFunc(int protoId, const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_GameProto_SERVER
	if (protoId < 0 || protoId >= 6)
		return;

	g_lrb_GameProto_reqFuncs[protoId >> 1] = func;
#endif
}

void bindAckFunc(AckFuncType acktype, const std::function<void()> &func)
{
#ifndef LRB_GameProto_SERVER
	g_lrb_GameProto_ackFuncs[(int)acktype] = func;
#endif
}

}
}

