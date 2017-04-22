#ifndef _LRB_SERVER_DATA_H
#define _LRB_SERVER_DATA_H

#include <string>

namespace lrb {

namespace server {
	
namespace mod {

	class ModRoleInfo;
	class ModModInfo;
	

	class RoleData {
	public:
		RoleData();
		~RoleData();
	
		void loadModRoleInfo(uint32_t roleId);
		ModRoleInfo *getModRoleInfo();

		ModModInfo *getModModInfo();
		ModModInfo *loadModModInfo();

	private:
		ModRoleInfo *m_roleInfo;
		ModModInfo *m_modInfo;
	};

	class DataCache {
	public:
		static DataCache *getInstance();

		DataCache();
		~DataCache();

		void initDataCache();
		uint32_t createRoleInfo();
		RoleData *getRoleData(uint32_t roleId);

	private:
		uint32_t m_nextRoleId;
	};

}

}

}

#endif
