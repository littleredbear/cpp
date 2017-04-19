#include "lrbModRoleInfo.h"
#include "lrbGameProto.h"
#include "lrbGameProtoFunc.h"
#include "lrbNetWork.h"
#include "lrbServerData.h"


using namespace lrb::server::mod;
using namespace lrb::GameProto;
using namespace lrb::NetWork;


extern ReqVerifyData g_lrb_GameProto_ReqVerifyData;
extern ReqRoleInfo g_lrb_GameProto_ReqRoleInfo;
extern ReqRoleName g_lrb_GameProto_ReqRoleName;
extern ReqRolePos g_lrb_GameProto_ReqRolePos;

void ModRoleInfo::initModRoleInfo()
{
	lrb::GameProto::bindReqVerifyDataFunc(std::bind(ModRoleInfo::reqVerifyDataFunc, std::placeholders::_1));
	lrb::GameProto::bindReqRoleInfoFunc(std::bind(ModRoleInfo::reqRoleInfoFunc, std::placeholders::_1));
	lrb::GameProto::bindReqRoleNameFunc(std::bind(ModRoleInfo::reqRoleNameFunc, std::placeholders::_1));
	lrb::GameProto::bindReqRolePosFunc(std::bind(ModRoleInfo::reqRolePosFunc, std::placeholders::_1));
}

void ModRoleInfo::reqVerifyDataFunc(lrb::NetWork::DataPacker *packer)
{
	lrb::GameProto::packAckVerifyData(packer, g_lrb_GameProto_ReqVerifyData.verify);
}

void ModRoleInfo::reqRoleInfoFunc(lrb::NetWork::DataPacker *packer)
{
	if (g_lrb_GameProto_ReqRoleInfo.roleId == 0)
		g_lrb_GameProto_ReqRoleInfo.roleId = DataCache::getInstance()->createRoleInfo();

	lrb::GameProto::packAckRoleInfo(packer, g_lrb_GameProto_ReqRoleInfo.roleId);
}

void ModRoleInfo::reqRoleNameFunc(lrb::NetWork::DataPacker *packer)
{
	RoleData *rdata = DataCache::getInstance()->getRoleData(g_lrb_GameProto_ReqRoleInfo.roleId);
	if (rdata == NULL)
	{
		lrb::GameProto::packAckRoleName(packer, "");
		return;
	}
		
	if (!g_lrb_GameProto_ReqRoleName.name.empty())
	{
		rdata->getModRoleInfo()->setName(g_lrb_GameProto_ReqRoleName.name);
	}

	lrb::GameProto::packAckRoleName(packer, rdata->getModRoleInfo()->name());
}

void ModRoleInfo::reqRolePosFunc(lrb::NetWork::DataPacker *packer)
{
	RoleData *rdata = DataCache::getInstance()->getRoleData(g_lrb_GameProto_ReqRoleInfo.roleId);
	if (rdata == NULL)
	{
		lrb::GameProto::packAckRolePos(packer, -1, -1);
		return;
	}

	if (g_lrb_GameProto_ReqRolePos.posx != -1 &&
	    g_lrb_GameProto_ReqRolePos.posy != -1)
	{
		rdata->getModRoleInfo()->setPosX(g_lrb_GameProto_ReqRolePos.posx);
		rdata->getModRoleInfo()->setPosY(g_lrb_GameProto_ReqRolePos.posy);
	}

	lrb::GameProto::packAckRolePos(packer, rdata->getModRoleInfo()->posx(), rdata->getModRoleInfo()->posy());
}


ModRoleInfo::ModRoleInfo():
m_roleId(0),
m_posx(-1),
m_posy(-1)
{

}

ModRoleInfo::~ModRoleInfo()
{

}

void ModRoleInfo::loadMod(uint32_t roleId)
{
	setRoleId(roleId);
}

const std::string &ModRoleInfo::name()
{
	return m_name;
}

uint32_t ModRoleInfo::roleId()
{
	return m_roleId;
}

uint32_t ModRoleInfo::posx()
{
	return m_posx;
}

uint32_t ModRoleInfo::posy()
{
	return m_posy;
}

void ModRoleInfo::setName(const std::string &name)
{
	m_name = name;
}

void ModRoleInfo::setRoleId(uint32_t roleId)
{
	m_roleId = roleId;
}

void ModRoleInfo::setPosX(uint32_t posx)
{
	m_posx = posx;
}

void ModRoleInfo::setPosY(uint32_t posy)
{
	m_posy = posy;
}







