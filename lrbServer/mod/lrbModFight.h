#ifndef _LRB_MOD_FIGHT_H
#define _LRB_MOD_FIGHT_H


#include <map>
#include <vector>


namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace server {

namespace mod {

	class FightOption {
	public:
		FightOption();
		~FightOption();

		uint32_t targetId();
		void setTargetId(uint32_t targetId);

		uint32_t itemId();
		void setItemId(uint32_t itemId);

	private:
		uint32_t m_targetId;
		uint32_t m_itemId;

	};

	class FightBuffData {
	public:
		FightBuffData();
		~FightBuffData();

		uint32_t endRound();
		void setEndRound(uint32_t endRound);

		uint32_t value();
		void setValue(uint32_t value);

	private:
		uint32_t m_endRound;
		uint32_t m_value;
	};

	class FightRoleData {
	public:
		FightRoleData();
		~FightRoleData();

		uint32_t fightRoleId();
		void setFightRoleId(uint32_t fightRoleId);
		
		uint32_t itemId();
		void setItemId(uint32_t itemId);
		
		uint32_t hp();
		void setHP(uint32_t hp);
		void addHP(uint32_t hp);

	private:
		std::map<uint32_t, FightBuffData> m_buffs;
	
		uint32_t m_fightRoleId;
		uint32_t m_itemId;
		uint32_t m_hp;

	};

//--------------------------------------ModFightData------------------------------
	
	class ModFightData {
	public:
		ModFightData();
		~ModFightData();
		
		int getFightData(void **res);

		void addFightOption(uint32_t itemId, uint32_t userId, uint32_t targetId);
		void resetFightRound();
		int processFightRound(void **res);

		void loadFightRoles();

		uint32_t modId();
		void setModId(uint32_t modId);

		uint32_t modRoleId();
		void setModRoleId(uint32_t modRoleId);
		
		uint32_t modTargetId();
		void setModTargetId(uint32_t modTargetId);
		
		uint32_t roundId();
		uint32_t nextRound();
		
	private:
		std::map<uint32_t, FightRoleData> m_fightRoles;
		std::vector<uint32_t> m_fightRoleIds;
		std::map<uint32_t, FightOption> m_options;

		uint32_t m_modId;
		uint32_t m_modRoleId;
		uint32_t m_modTargetId;
		uint32_t m_roundId;
	};

//--------------------------------------ModFight---------------------------------

	class ModFight {
	public:
		static void initModFight();

	private:
		static void reqFight(lrb::NetWork::DataPacker *packer);
		static void reqFightRound(lrb::NetWork::DataPacker *packer);

		static uint32_t nextFightId();

	};

}

}

}



#endif
