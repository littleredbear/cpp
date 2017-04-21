#include "lrbModChat.h"
#include "lrbGameProto.h"
#include "lrbGameProtoFunc.h"
#include "lrbServerNet.h"
#include "lrbNetWork.h"


using namespace lrb::server::mod;
using namespace lrb::GameProto;

extern ReqChatInfo g_lrb_GameProto_ReqChatInfo;
extern ReqStreamData g_lrb_GameProto_ReqStreamData;
extern ReqVerifyData g_lrb_GameProto_ReqVerifyData;
extern ReqRoleId g_lrb_GameProto_ReqRoleId;

void ModChat::initModChat()
{
	lrb::GameProto::bindReqChatInfoFunc(std::bind(ModChat::reqChatInfo, std::placeholders::_1));

}

void ModChat::reqChatInfo(lrb::NetWork::DataPacker *packer)
{

	lrb::GameProto::packAckStreamData(packer, g_lrb_GameProto_ReqStreamData.data, g_lrb_GameProto_ReqStreamData.size);
	lrb::GameProto::packAckChatInfo(packer, g_lrb_GameProto_ReqChatInfo.targetId, g_lrb_GameProto_ReqChatInfo.channelId);

	switch (g_lrb_GameProto_ReqChatInfo.channelId)
	{
	case 1://世界
	{
		packer->setGroupSend(LRB_SERVER_UDP_GROUP, LRB_SERVER_UDP_PORT);
	}
	break;

	case 2://个人
	{
		packer->sendToRoleIds(2, g_lrb_GameProto_ReqChatInfo.targetId, g_lrb_GameProto_ReqRoleId.roleId);
	}
	break;

	default:
	break;
	}

}

