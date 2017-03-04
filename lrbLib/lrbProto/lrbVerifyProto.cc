#include "lrbVerifyProto.h"


using namespace lrb::VerifyProto;

#ifdef LRB_VerifyProto_SERVER
ReqStreamData g_lrb_VerifyProto_ReqStreamData;
ReqVerifyData g_lrb_VerifyProto_ReqVerifyData;
void *g_lrb_VerifyProto_ptrs[] = {
&g_lrb_VerifyProto_ReqStreamData,
&g_lrb_VerifyProto_ReqVerifyData,
};
std::function<void(lrb::NetWork::DataPacker *)> g_lrb_VerifyProto_reqFuncs[4];
#else
AckStreamData g_lrb_VerifyProto_AckStreamData;
AckVerifyData g_lrb_VerifyProto_AckVerifyData;
void *g_lrb_VerifyProto_ptrs[] = {
&g_lrb_VerifyProto_AckStreamData,
&g_lrb_VerifyProto_AckVerifyData,
};
std::function<void()> g_lrb_VerifyProto_ackFuncs[(int)AckFuncType::AFT_TOP];
#endif

short g_lrb_VerifyProto_confs[][5] = {
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,0,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
};

namespace lrb {

namespace VerifyProto {

void *getUnpackDest(int protoId)
{
	if (protoId < 0 || protoId >= 4)
		return NULL;

#ifdef LRB_VerifyProto_SERVER
	if (protoId & 1)
#else
	if (!(protoId & 1))
#endif
		return NULL;

	return g_lrb_VerifyProto_ptrs[protoId >> 1];
}

short *getProtoConfs(int protoId)
{
	if (protoId < 0 || protoId >= 4)
		return NULL;

	return g_lrb_VerifyProto_confs[protoId];
}

void execReqFunc(int protoId, lrb::NetWork::DataPacker *packer)
{
#ifdef LRB_VerifyProto_SERVER
	if (protoId < 0 || protoId >= 4 || (protoId & 1))
		return;

	g_lrb_VerifyProto_reqFuncs[protoId >> 1](packer);
#endif
}

void execAckFunc()
{
#ifndef LRB_VerifyProto_SERVER
	g_lrb_VerifyProto_ackFuncs[g_lrb_VerifyProto_AckVerifyData.verify]();
#endif
}

void bindReqFunc(int protoId, const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_VerifyProto_SERVER
	if (protoId < 0 || protoId >= 4)
		return;

	g_lrb_VerifyProto_reqFuncs[protoId >> 1] = func;
#endif
}

void bindAckFunc(AckFuncType acktype, const std::function<void()> &func)
{
#ifndef LRB_VerifyProto_SERVER
	g_lrb_VerifyProto_ackFuncs[(int)acktype] = func;
#endif
}

}
}

