#ifndef _LRB_SERVER_DATA_H
#define _LRB_SERVER_DATA_H

#include <string>

namespace lrb {

namespace server {
	
	class ModRoleInfo {
	public:
		ModRoleInfo();
		~ModRoleInfo();
		
		void loadMod(uint32_t roleId);

		std::string name();
		uint32_t roleId();
		uint32_t posx();
		uint32_t posy();

		void setName(const std::string &name);
		void setPosX(uint32_t posx);
		void setPosY(uint32_t posy);

	private:
		std::string m_name;
		uint32_t m_roleId;
		uint32_t m_posx;
		uint32_t m_posy;
	};

	class RoleData {
	public:
		RoleData();
		~RoleData();
	
		void loadModRoleInfo(uint32_t roleId);
		ModRoleInfo *getRoleInfo();

	private:
		ModRoleInfo *m_roleInfo;
		
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

#endif
