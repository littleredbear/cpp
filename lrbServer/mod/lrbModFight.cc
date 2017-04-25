#include "lrbModFight.h"
#include "lrbGameProto.h"
#include "lrbGameProtoFunc.h"
#include "lrbErrCode.h"
#include "lrbModData.h"
#include "lrbModModInfo.h"
#include "lrbModRoleInfo.h"
#include "lrbNetWork.h"
#include <unordered_map>


using namespace lrb::server::mod;
using namespace lrb::GameProto;


extern ReqStreamData g_lrb_GameProto_ReqStreamData;
extern ReqFight g_lrb_GameProto_ReqFight;
extern ReqFightRound g_lrb_GameProto_ReqFightRound;
extern ReqUseItem g_lrb_GameProto_ReqUseItem;


namespace {
	std::unordered_map<uint32_t, ModFightData *> s_lrb_fightdata_map;
	uint32_t s_lrb_fightid;
	ErrCode s_lrb_fighterr;
}


FightOption::FightOption():
m_targetId(0),
m_itemId(0)
{

}

FightOption::~FightOption()
{

}

uint32_t FightOption::targetId()
{
	return m_targetId;
}

void FightOption::setTargetId(uint32_t targetId)
{
	m_targetId = targetId;
}

uint32_t FightOption::itemId()
{
	return m_itemId;
}

void FightOption::setItemId(uint32_t itemId)
{
	m_itemId = itemId;
}


FightBuffData::FightBuffData():
m_endRound(0),
m_value(0)
{

}

FightBuffData::~FightBuffData()
{

}

uint32_t FightBuffData::endRound()
{
	return m_endRound;
}

void FightBuffData::setEndRound(uint32_t endRound)
{
	m_endRound = endRound;
}

uint32_t FightBuffData::value()
{
	return m_value;
}

void FightBuffData::setValue(uint32_t value)
{
	m_value = value;
}



FightRoleData::FightRoleData():
m_fightRoleId(0),
m_itemId(0),
m_hp(0)
{

}

FightRoleData::~FightRoleData()
{

}

uint32_t FightRoleData::fightRoleId()
{
	return m_fightRoleId;
}

void FightRoleData::setFightRoleId(uint32_t fightRoleId)
{
	m_fightRoleId = fightRoleId;
}

uint32_t FightRoleData::itemId()
{
	return m_itemId;
}

void FightRoleData::setItemId(uint32_t itemId)
{
	m_itemId = itemId;
}

uint32_t FightRoleData::hp()
{
	return m_hp;
}

void FightRoleData::setHP(uint32_t hp)
{
	m_hp = hp;
}

void FightRoleData::addHP(uint32_t hp)
{
	m_hp += hp;
}


//-----------------------------------ModFightData----------------------------


ModFightData::ModFightData():
m_modId(0),
m_modRoleId(0),
m_modTargetId(0),
m_roundId(1)
{

}

ModFightData::~ModFightData()
{

}

int ModFightData::getFightData(void **res)
{
	lrb::NetWork::DataPacker packer;

	for (auto &role : m_fightRoles)
	{
		lrb::GameProto::packAckFightRoleData(&packer, role.second.fightRoleId(), role.second.itemId(), role.second.hp());
	}

	return packer.getData(res);
}

void ModFightData::addFightOption(uint32_t itemId, uint32_t userId, uint32_t targetId)
{
	m_options[userId].setTargetId(targetId);
	m_options[userId].setItemId(itemId);
}

void ModFightData::resetFightRound()
{
	m_options.clear();
}

int ModFightData::processFightRound(void **res)
{
	lrb::NetWork::DataPacker packer;
	
	for (auto roleId : m_fightRoleIds)
	{
		auto &role = m_fightRoles[roleId];
		if (role.hp() == 0)
			continue;
		
		auto iter = m_options.find(roleId);
		if (iter == m_options.end())
			continue;

		lrb::GameProto::packAckFightAttack(&packer, roleId, iter->second.itemId(), role.hp());

		if (iter->second.itemId() == 1)
		{
			auto target = m_fightRoles.find(iter->second.targetId());
			if (target == m_fightRoles.end())
				continue;

			target->second.addHP(-1);
			lrb::GameProto::packAckFightDefend(&packer, iter->second.targetId(), target->second.hp());

		}

	}

	return packer.getData(res);
}

void ModFightData::loadFightRoles()
{
	
}

uint32_t ModFightData::modId()
{
	return m_modId;
}

void ModFightData::setModId(uint32_t modId)
{
	m_modId = modId;
}

uint32_t ModFightData::modRoleId()
{
	return m_modRoleId;
}

void ModFightData::setModRoleId(uint32_t modRoleId)
{
	m_modRoleId = modRoleId;
}

uint32_t ModFightData::modTargetId()
{
	return m_modTargetId;
}

void ModFightData::setModTargetId(uint32_t modTargetId)
{
	m_modTargetId = modTargetId;
}

uint32_t ModFightData::roundId()
{
	return m_roundId;
}

uint32_t ModFightData::nextRound()
{
	return ++m_roundId;
}


//-----------------------------------ModFight---------------------------------

void ModFight::initModFight()
{
	lrb::GameProto::bindReqFightFunc(std::bind(ModFight::reqFight, std::placeholders::_1));
	lrb::GameProto::bindReqFightRoundFunc(std::bind(ModFight::reqFightRound, std::placeholders::_1));

}

void ModFight::reqFight(lrb::NetWork::DataPacker *packer)
{
	if (packer->state() != 0)
		return;

	RoleData *rdata = DataCache::getInstance()->getRoleData(packer->netLink()->roleId());
	if (rdata == NULL)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_RoleId);
		return;
	}

	if (rdata->getModRoleInfo()->fightId() != 0)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_InFight);
		return;
	}

	ModModInfo *modInfo = rdata->getModModInfo();
	if (modInfo == NULL)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_NoModInfo);
		return;
	}
		
	ModFightData *fdata = new ModFightData;
	fdata->setModId(modInfo->modId());
	fdata->setModRoleId(modInfo->modRoleId());
	fdata->setModTargetId(g_lrb_GameProto_ReqFight.modTargetId);
	fdata->loadFightRoles();

	void *res;
	int ret = fdata->getFightData(&res);

	if (ret == -1)
	{
		delete fdata;
		lrb::server::sendErrCode(packer, ErrCode::Err_FightData);
	} else 
	{
		uint32_t fightId = ModFight::nextFightId();

		s_lrb_fightdata_map[fightId] = fdata;
		rdata->getModRoleInfo()->setFightId(fightId);

		lrb::GameProto::packAckStreamData(packer, res, ret);
		lrb::GameProto::packAckFight(packer);
		packer->addValue();
		
		free(res);
	}
}

void ModFight::reqFightRound(lrb::NetWork::DataPacker *packer)
{
	if (packer->state() != 0)
		return;
		
	RoleData *rdata = DataCache::getInstance()->getRoleData(packer->netLink()->roleId());

	if (rdata == NULL)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_RoleId);
		return;
	}

	auto iter = s_lrb_fightdata_map.find(rdata->getModRoleInfo()->fightId());
	if (iter == s_lrb_fightdata_map.end())
	{
		rdata->getModRoleInfo()->setFightId(0);
		lrb::server::sendErrCode(packer, ErrCode::Err_FightId);
		return;
	}
	
	iter->second->resetFightRound();

	void *data = g_lrb_GameProto_ReqStreamData.data;
	int len;
	memcpy(&len, data, sizeof(int));
	data += sizeof(int);

	while (len > 0)
	{
		int plen;
		memcpy(&plen, data, sizeof(int));
		data += sizeof(int);
		len -= sizeof(int);

		int uuid = lrb::NetWork::unpackData(data, plen, lrb::NetWork::ProtoType::PT_GameProto);
		data += plen;
		len -= plen;

		if (uuid == -1)
			continue;
		
		iter->second->addFightOption(g_lrb_GameProto_ReqUseItem.itemId, g_lrb_GameProto_ReqUseItem.userId, g_lrb_GameProto_ReqUseItem.targetId);

	}
	
	void *res;
	int ret = iter->second->processFightRound(&res);
	if (ret == -1)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_FightRound);
	} else
	{
		lrb::GameProto::packAckStreamData(packer, res, ret);
		lrb::GameProto::packAckFightRound(packer, iter->second->nextRound());
		packer->addValue(2);
		free(res);
	}

}

uint32_t ModFight::nextFightId()
{
	if (++s_lrb_fightid == 0)
		++s_lrb_fighid;

	return s_lrb_fightid;
}




