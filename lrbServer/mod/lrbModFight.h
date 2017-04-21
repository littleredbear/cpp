#ifndef _LRB_MOD_FIGHT_H
#define _LRB_MOD_FIGHT_H



namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace server {

namespace mod {

	class ModFightData {

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
