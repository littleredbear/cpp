#include "lrbModRoleInfo.h"
#include "lrbGameProto.h"
#include "lrbNetWork.h"
#include "lrbServerData.h"


using namespace lrb::server::mod;
using namespace lrb::GameProto;
using namespace lrb::NetWork;


extern ReqVerifyData g_lrb_GameProto_ReqVerifyData;
extern ReqRoleInfo g_lrb_GameProto_ReqRoleInfo;
extern ReqRoleName g_lrb_GameProto_ReqRoleName;
extern ReqRolePos g_lrb_GameProto_ReqRolePos;

void RoleInfo::initModRoleInfo()
{
	lrb::GameProto::bindReqFunc(2, std::bind(RoleInfo::reqVerifyDataFunc, std::placeholders::_1));
	lrb::GameProto::bindReqFunc(4, std::bind(RoleInfo::reqRoleInfoFunc, std::placeholders::_1));
	lrb::GameProto::bindReqFunc(6, std::bind(RoleInfo::reqRoleNameFunc, std::placeholders::_1));
	lrb::GameProto::bindReqFunc(8, std::bind(RoleInfo::reqRolePosFunc, std::placeholders::_1));
}

void RoleInfo::reqVerifyDataFunc(lrb::NetWork::DataPacker *packer)
{
	AckVerifyData vdata;
	vdata.verify = g_lrb_GameProto_ReqVerifyData.verify;

	packer->packData(&vdata, 3, ProtoType::PT_GAME);
}

void RoleInfo::reqRoleInfoFunc(lrb::NetWork::DataPacker *packer)
{
	AckRoleInfo rdata;
	if (g_lrb_GameProto_ReqRoleInfo.roleId == 0)
	{
		rdata.roleId = DataCache::getInstance()->createRoleInfo();
	} else 
	{
		rdata.roleId = g_lrb_GameProto_ReqRoleInfo.roleId;
	}
	packer->packData(&rdata, 5, ProtoType::PT_GAME);
}

void RoleInfo::reqRoleNameFunc(lrb::NetWork::DataPacker *packer)
{
	AckRoleName ndata;
	RoleData *rdata = DataCache::getInstance()->getRoleData(g_lrb_GameProto_ReqRoleInfo.roleId);
	if (rdata == NULL)
	{
		packer->packData(NULL, 5, ProtoType::PT_GAME);
		return;
	}
		
	if (g_lrb_GameProto_ReqRoleName.name.empty())
	{
		ndata.name = rdata->getRoleInfo()->name();
	} else
	{
		rdata->getRoleInfo()->setName(g_lrb_GameProto_ReqRoleName.name);
		ndata.name = g_lrb_GameProto_ReqRoleName.name;
	}
	packer->packData(&ndata, 5, ProtoType::PT_GAME);
}

void RoleInfo::reqRolePosFunc(lrb::NetWork::DataPacker *packer)
{
	AckRolePos pdata;
	RoleData *rdata = DataCache::getInstance()->getRoleData(g_lrb_GameProto_ReqRoleInfo.roleId);
	if (rdata == NULL)
	{
		packer->packData(NULL, 7, ProtoType::PT_GAME);
		return;
	}

	if (g_lrb_GameProto_ReqRolePos.posx == -1 ||
	    g_lrb_GameProto_ReqRolePos.posy == -1)
	{
		pdata.posx = rdata->getRoleInfo()->posx();
		pdata.posy = rdata->getRoleInfo()->posy();
	} else
	{
		rdata->getRoleInfo()->setPosX(g_lrb_GameProto_ReqRolePos.posx);
		rdata->getRoleInfo()->setPosY(g_lrb_GameProto_ReqRolePos.posy);
		pdata.posx = g_lrb_GameProto_ReqRolePos.posx;
		pdata.posy = g_lrb_GameProto_ReqRolePos.posy;
	}
	packer->packData(&pdata, 7, ProtoType::PT_GAME);
}




