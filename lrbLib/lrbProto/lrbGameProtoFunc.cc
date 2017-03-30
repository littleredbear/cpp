#include "lrbGameProtoFunc.h"
#include "lrbNetWork.h"

using namespace lrb::GameProto;

#ifdef LRB_GameProto_SERVER
std::function<void(lrb::NetWork::DataPacker *)> g_lrb_GameProto_reqFuncs[7];
#else
std::function<void()> g_lrb_GameProto_ackFuncs[(int)AckFuncType::AFT_TOP];
extern AckVerifyData g_lrb_GameProto_AckVerifyData;
#endif

namespace lrb {

namespace GameProto {

void packReqStreamData(lrb::NetWork::DataPacker *packer, void *data, int32_t size)
{
	ReqStreamData tmpdata;
	tmpdata.data = data;
	tmpdata.size = size;
	packer->packData(&tmpdata, 0, lrb::NetWork::ProtoType::PT_GameProto);
}

void bindReqStreamDataFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_GameProto_SERVER
	g_lrb_GameProto_reqFuncs[0] = func;
#endif
}

void packAckStreamData(lrb::NetWork::DataPacker *packer, void *data, int32_t size)
{
	AckStreamData tmpdata;
	tmpdata.data = data;
	tmpdata.size = size;
	packer->packData(&tmpdata, 1, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify)
{
	ReqVerifyData tmpdata;
	tmpdata.verify = verify;
	packer->packData(&tmpdata, 2, lrb::NetWork::ProtoType::PT_GameProto);
}

void bindReqVerifyDataFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_GameProto_SERVER
	g_lrb_GameProto_reqFuncs[1] = func;
#endif
}

void packAckVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify)
{
	AckVerifyData tmpdata;
	tmpdata.verify = verify;
	packer->packData(&tmpdata, 3, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqErrorCode(lrb::NetWork::DataPacker *packer, uint32_t errorCode)
{
	ReqErrorCode tmpdata;
	tmpdata.errorCode = errorCode;
	packer->packData(&tmpdata, 4, lrb::NetWork::ProtoType::PT_GameProto);
}

void bindReqErrorCodeFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_GameProto_SERVER
	g_lrb_GameProto_reqFuncs[2] = func;
#endif
}

void packAckErrorCode(lrb::NetWork::DataPacker *packer, uint32_t errorCode)
{
	AckErrorCode tmpdata;
	tmpdata.errorCode = errorCode;
	packer->packData(&tmpdata, 5, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId)
{
	ReqRoleInfo tmpdata;
	tmpdata.roleId = roleId;
	packer->packData(&tmpdata, 6, lrb::NetWork::ProtoType::PT_GameProto);
}

void bindReqRoleInfoFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_GameProto_SERVER
	g_lrb_GameProto_reqFuncs[3] = func;
#endif
}

void packAckRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId)
{
	AckRoleInfo tmpdata;
	tmpdata.roleId = roleId;
	packer->packData(&tmpdata, 7, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqRoleName(lrb::NetWork::DataPacker *packer, const std::string& name)
{
	ReqRoleName tmpdata;
	tmpdata.name = name;
	packer->packData(&tmpdata, 8, lrb::NetWork::ProtoType::PT_GameProto);
}

void bindReqRoleNameFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_GameProto_SERVER
	g_lrb_GameProto_reqFuncs[4] = func;
#endif
}

void packAckRoleName(lrb::NetWork::DataPacker *packer, const std::string& name)
{
	AckRoleName tmpdata;
	tmpdata.name = name;
	packer->packData(&tmpdata, 9, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy)
{
	ReqRolePos tmpdata;
	tmpdata.posx = posx;
	tmpdata.posy = posy;
	packer->packData(&tmpdata, 10, lrb::NetWork::ProtoType::PT_GameProto);
}

void bindReqRolePosFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_GameProto_SERVER
	g_lrb_GameProto_reqFuncs[5] = func;
#endif
}

void packAckRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy)
{
	AckRolePos tmpdata;
	tmpdata.posx = posx;
	tmpdata.posy = posy;
	packer->packData(&tmpdata, 11, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqUseItem(lrb::NetWork::DataPacker *packer, uint32_t itemId, uint32_t userId, uint32_t targetId)
{
	ReqUseItem tmpdata;
	tmpdata.itemId = itemId;
	tmpdata.userId = userId;
	tmpdata.targetId = targetId;
	packer->packData(&tmpdata, 12, lrb::NetWork::ProtoType::PT_GameProto);
}

void bindReqUseItemFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func)
{
#ifdef LRB_GameProto_SERVER
	g_lrb_GameProto_reqFuncs[6] = func;
#endif
}

void packAckUseItem(lrb::NetWork::DataPacker *packer)
{
	AckUseItem tmpdata;
	packer->packData(&tmpdata, 13, lrb::NetWork::ProtoType::PT_GameProto);
}

void bindAckFunc(AckFuncType acktype, const std::function<void()> &func)
{
#ifndef LRB_GameProto_SERVER
	if (acktype > AckFuncType::AFT_BOT && acktype < AckFuncType::AFT_TOP)
		g_lrb_GameProto_ackFuncs[(int)acktype] = func;
#endif
}

void execReqFunc(int protoId, lrb::NetWork::DataPacker *packer)
{
#ifdef LRB_GameProto_SERVER
	if (protoId >= 0 && protoId < 14 && !(protoId & 1))
		g_lrb_GameProto_reqFuncs[protoId >> 1](packer);
#endif
}

void execAckFunc()
{
#ifndef LRB_GameProto_SERVER
	int verify = g_lrb_GameProto_AckVerifyData.verify;
	if(verify > (int)AckFuncType::AFT_BOT && verify < (int)AckFuncType::AFT_TOP)
		g_lrb_GameProto_ackFuncs[verify]();
#endif
}


}

}

