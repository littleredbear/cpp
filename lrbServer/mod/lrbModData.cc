#include "lrbModData.h"
#include "lrbModRoleInfo.h"
#include <unordered_map>


using namespace lrb::server::mod;


namespace {
	
	DataCache s_lrb_server_datacache_instance;
	std::unordered_map<uint32_t, RoleData *> s_lrb_server_rolemap;

}

RoleData::RoleData():
m_roleInfo(NULL),
m_modInfo(NULL)
{

}

RoleData::~RoleData()
{
	delete m_roleInfo;
	delete m_modInfo;
}

void RoleData::loadModRoleInfo(uint32_t roleId)
{
	delete m_roleInfo;
	m_roleInfo = new ModRoleInfo;
	m_roleInfo->loadMod(roleId);
}

ModRoleInfo *RoleData::getModRoleInfo()
{
	return m_roleInfo;
}

ModModInfo *RoleData::getModModInfo()
{
	return m_modInfo;
}

ModModInfo *RoleData::loadModModInfo()
{
	if (m_modInfo == NULL)
		m_modInfo = new ModModInfo;

	return m_modInfo;
}

DataCache *DataCache::getInstance()
{
	return &s_lrb_server_datacache_instance;
}

DataCache::DataCache():
m_nextRoleId(1)
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




