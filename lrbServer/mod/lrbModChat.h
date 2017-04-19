#ifndef _LRB_MOD_CHAT_H
#define _LRB_MOD_CHAT_H


namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace server {

namespace mod {

	class ModChat {
	public:
		static void initModChat();

	private:
		static void reqChatInfo(lrb::NetWork::DataPacker *packer);

	};

}

}

}

#endif
