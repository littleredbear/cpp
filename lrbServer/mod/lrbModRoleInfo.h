#ifndef _LRB_MOD_ROLE_INFO_H
#define _LRB_MOD_ROLE_INFO_H


namespace lrb {

namespace NetWork {
	class DataPacker;
}
	
namespace server {

namespace mod {

	class RoleInfo {
	public:
		static void initModRoleInfo();

	private:
		static void reqVerifyDataFunc(lrb::NetWork::DataPacker *packer);
		static void reqRoleInfoFunc(lrb::NetWork::DataPacker *packer);
		static void reqRoleNameFunc(lrb::NetWork::DataPacker *packer);
		static void reqRolePosFunc(lrb::NetWork::DataPacker *packer);
		
	};

}

}

}


#endif
