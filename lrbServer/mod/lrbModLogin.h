#ifndef _LRB_MOD_LOGIN_H
#define _LRB_MOD_LOGIN_H


namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace server {

namespace mod {

	class ModLogin {
	public:
		static void initModLogin();
	
	private:
		static void reqLogin(lrb::NetWork::DataPacker *packer);

	};

}

}

}


#endif
