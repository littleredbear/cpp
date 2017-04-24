#ifndef _LRB_MOD_FIGHT_H
#define _LRB_MOD_FIGHT_H


#include <vector>


namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace server {

namespace mod {

	class FightRoleData {

	};
	
	class ModFightData {
	public:
		

	private:

		uint32_t m_modId;
		uint32_t m_modRoleId;
		uint32_t m_modTargetId;
	};

	class ModFight {
	public:
		static void initModFight();

	private:
		static void reqFight(lrb::NetWork::DataPacker *packer);

	};

}

}

}



#endif
