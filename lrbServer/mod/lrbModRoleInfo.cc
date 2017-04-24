#include "lrbModRoleInfo.h"
#include "lrbGameProto.h"
#include "lrbGameProtoFunc.h"
#include "lrbNetWork.h"
#include "lrbModData.h"
#include "lrbErrCode.h"
#include <random>


using namespace lrb::server::mod;
using namespace lrb::GameProto;
using namespace lrb::NetWork;


extern ReqAckFuncType g_lrb_GameProto_ReqAckFuncType;
extern ReqVerifyData g_lrb_GameProto_ReqVerifyData;
extern ReqRoleId g_lrb_GameProto_ReqRoleId;
extern ReqRoleName g_lrb_GameProto_ReqRoleName;
extern ReqRolePos g_lrb_GameProto_ReqRolePos;

void ModRoleInfo::initModRoleInfo()
{
	lrb::GameProto::bindReqAckFuncTypeFunc(std::bind(ModRoleInfo::reqAckFuncTypeFunc, std::placeholders::_1));
	lrb::GameProto::bindReqVerifyDataFunc(std::bind(ModRoleInfo::reqVerifyDataFunc, std::placeholders::_1));
	lrb::GameProto::bindReqRoleIdFunc(std::bind(ModRoleInfo::reqRoleIdFunc, std::placeholders::_1));
	lrb::GameProto::bindReqRoleNameFunc(std::bind(ModRoleInfo::reqRoleNameFunc, std::placeholders::_1));
	lrb::GameProto::bindReqRolePosFunc(std::bind(ModRoleInfo::reqRolePosFunc, std::placeholders::_1));

}

void ModRoleInfo::reqAckFuncTypeFunc(lrb::NetWork::DataPacker *packer)
{
	lrb::GameProto::packAckFuncType(packer, g_lrb_GameProto_ReqAckFuncType.acktype);
}

void ModRoleInfo::reqVerifyDataFunc(lrb::NetWork::DataPacker *packer)
{
	RoleData *rdata = DataCache::getInstance()->getRoleData(packer->netLink()->roleId());

	if (g_lrb_GameProto_ReqVerifyData.verify != rdata->getModRoleInfo()->verify())
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_Verify);
	} else
	{
		lrb::GameProto::packAckVerifyData(packer, rdata->getModRoleInfo()->nextVerify());
	}
}

void ModRoleInfo::reqRoleIdFunc(lrb::NetWork::DataPacker *packer)
{
	if (g_lrb_GameProto_ReqRoleId.roleId == 0)
		g_lrb_GameProto_ReqRoleId.roleId = DataCache::getInstance()->createRoleInfo();

	lrb::GameProto::packAckRoleInfo(packer, g_lrb_GameProto_ReqRoleId.roleId);
}

void ModRoleInfo::reqRoleNameFunc(lrb::NetWork::DataPacker *packer)
{
	if (packer->state() != 0)
		return;

	RoleData *rdata = DataCache::getInstance()->getRoleData(g_lrb_GameProto_ReqRoleId.roleId);
	if (rdata == NULL)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_RoleId);
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
	if (packer->state() != 0)
		return;

	RoleData *rdata = DataCache::getInstance()->getRoleData(g_lrb_GameProto_ReqRoleId.roleId);
	if (rdata == NULL)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_RoleId);
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

uint32_t ModRoleInfo::verify()
{
	return m_verify;
}

uint32_t ModRoleInfo::nextVerify()
{
	std::random_device rd;
	std::uniform_int_distribution<uint32_t> uni_dist(0, -1);
	m_verify = uni_dist(rd);

	return m_verify;
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







