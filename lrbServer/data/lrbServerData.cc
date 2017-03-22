#include "lrbServerData.h"
#include <unordered_map>


using namespace lrb::server;


namespace {
	
	DataCache s_lrb_server_datacache_instance;
	std::unordered_map<uint32_t, RoleData *> s_lrb_server_rolemap;
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
	m_roleId = roleId;
}

std::string ModRoleInfo::name()
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

void  ModRoleInfo::setPosX(uint32_t posx)
{
	m_posx = posx;
}

void ModRoleInfo::setPosY(uint32_t posy)
{
	m_posy = posy;
}

RoleData::RoleData():
m_roleInfo(NULL)
{

}

RoleData::~RoleData()
{
	delete m_roleInfo;
}

void RoleData::loadModRoleInfo(uint32_t roleId)
{
	delete m_roleInfo;
	m_roleInfo = new ModRoleInfo;
	m_roleInfo->loadMod(roleId);
}

ModRoleInfo *RoleData::getRoleInfo()
{
	return m_roleInfo;
}


DataCache *DataCache::getInstance()
{
	return &s_lrb_server_datacache_instance;
}

DataCache::DataCache():
m_nextRoleId(0)
{

}

DataCache::~DataCache()
{

}

void DataCache::initDataCache()
{
	m_nextRoleId = 1;
}

uint32_t DataCache::createRoleInfo()
{
	return m_nextRoleId++;
}

RoleData *DataCache::getRoleData(uint32_t roleId)
{
	if (roleId >= m_nextRoleId)
		return NULL;

	auto iter = s_lrb_server_rolemap.find(roleId);
	if (iter != s_lrb_server_rolemap.end())
		return iter->second;

	RoleData *data = new RoleData;
	data->loadModRoleInfo(roleId);
	s_lrb_server_rolemap.insert(std::make_pair(roleId, data));
	
	return data;
}






