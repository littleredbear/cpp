#include "lrbModModInfo.h"
#include "lrbNetWork.h"
#include "lrbGameProto.h"
#include "lrbModData.h"
#include "lrbModEnum.h"
#include <unordered_map>


using namespace lrb::server::mod;
using namespace lrb::GameProto;


extern ReqModId g_lrb_GameProto_ReqModId;


namespace {

	std::unordered_map<uint32_t, uint32_t> s_lrb_modinfo_map[(uint32_t)ModType::MT_Top - 1];
	uint32_t s_lrb_modinfo_roleid[(uint32_t)ModType::MT_Top - 1];

}


void ModModInfo::initModModInfo()
{
	lrb::GameProto::bindReqModId(std::bind(ModModInfo::reqModId, std::placeholders::_1));
}

void ModModInfo::reqModId(lrb::NetWork::DataPacker *packer)
{
	if (packer->state() != 0)
		return;

	if (g_lrb_GameProto_ReqModId.modId == 0 || 
	    g_lrb_GameProto_ReqModId.modId >= (uint32_t)ModType::MT_Top)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_ModId);
		return;
	}

	uint32_t roleId = packer->netLink()->roleId();
	RoleData *rdata = DataCache::getInstance()->getRoleData(roleId);
	if (rdata == NULL)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_RoleId);
		return;
	}

	ModModInfo *modInfo = rdata->loadModModInfo();
	if (modInfo == NULL)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_Memory);
		return;
	}

	if (modInfo->modRoleId() != 0)
	{
		s_lrb_modinfo_map.erase(modInfo->modRoleId());
	}

	modInfo->setModId(g_lrb_GameProto_ReqModId.modId);
	modInfo->setModRoleId(ModModInfo::getModRoleId(g_lrb_GameProto_ReqModId.modId));

	s_lrb_modinfo_map[modInfo->modRoleId()] = roleId;

	lrb::GameProto::packAckModId(packer, modInfo->modId(), modInfo->modRoleId());
	
}

uint32_t ModModInfo::getModRoleId(uint32_t modId)
{
	return ++s_lrb_modinfo_roleid[modId - 1];
}

ModModInfo::ModModInfo():
m_modId(0),
m_modRoleId(0)
{

}

ModModInfo::~ModModInfo()
{

}

uint32_t ModModInfo::modId()
{
	return m_modId;
}

void ModModInfo::setModId(uint32_t modId)
{
	m_modId = modId;
}

uint32_t ModModInfo::modRoleId()
{
	return m_modRoleId;
}

void ModModInfo::setModRoleId(uint32_t roleId)
{
	m_modRoleId = roleId;
}

