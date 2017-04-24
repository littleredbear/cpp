#ifndef _LRB_MOD_MOD_INFO_H
#define _LRB_MOD_MOD_INFO_H


namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace server {

namespace mod {

	class ModModInfo {
	public:
		static void initModModInfo();

		ModModInfo();
		~ModModInfo();

		uint32_t modId();
		void setModId(uint32_t modId);

		uint32_t modRoleId();
		void setModRoleId(uint32_t roleId);
	
	private:
		static void reqModId(lrb::NetWork::DataPacker *packer);
		static uint32_t getModRoleId(uint32_t modId);
		
		uint32_t m_modId;
		uint32_t m_modRoleId;
	
	};

}

}

}


#endif
