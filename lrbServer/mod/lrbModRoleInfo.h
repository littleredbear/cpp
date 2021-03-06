#ifndef _LRB_MOD_ROLE_INFO_H
#define _LRB_MOD_ROLE_INFO_H

#include <string>

namespace lrb {

namespace NetWork {
	class DataPacker;
}
	
namespace server {

namespace mod {

	class ModRoleInfo {
	public:
		static void initModRoleInfo();

		ModRoleInfo();
		~ModRoleInfo();
	
		void loadMod(uint32_t roleId);
		const std::string &name();
		uint32_t roleId();
		uint32_t posx();
		uint32_t posy();

		uint32_t fightId();
		void setFightId(uint32_t fightId);

		uint32_t nextVerify();

	private:
		static void reqAckFuncTypeFunc(lrb::NetWork::DataPacker *packer);
		static void reqVerifyDataFunc(lrb::NetWork::DataPacker *packer);
		static void reqRoleIdFunc(lrb::NetWork::DataPacker *packer);
		static void reqRoleNameFunc(lrb::NetWork::DataPacker *packer);
		static void reqRolePosFunc(lrb::NetWork::DataPacker *packer);
		
		uint32_t verify();

		void setName(const std::string &name);
		void setRoleId(uint32_t roleId);
		void setPosX(uint32_t posx);
		void setPosY(uint32_t posy);

		std::string m_name;
		uint32_t m_verify;
		uint32_t m_fightId;
		uint32_t m_roleId;
		uint32_t m_posx;
		uint32_t m_posy;

	};

}

}

}


#endif
