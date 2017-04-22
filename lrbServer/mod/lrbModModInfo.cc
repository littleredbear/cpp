#include "lrbModModInfo.h"
#include "lrbNetWork.h"
#include "lrbGameProto.h"


using namespace lrb::server::mod;
using namespace lrb::GameProto;


extern ReqModId g_lrb_GameProto_ReqModId;


void ModModInfo::initModModInfo()
{
	lrb::GameProto::bindReqModId(std::bind(ModModInfo::reqModId, std::placeholders::_1));
}

void ModModInfo::reqModId(lrb::NetWork::DataPacker *packer)
{
	uint32_t roleId = packer->netLink()->roleId();
	
	
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

