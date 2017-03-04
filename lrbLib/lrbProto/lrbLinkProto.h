#ifndef _LRB_LINK_PROTO_H
#define _LRB_LINK_PROTO_H

#include <string>
#include <functional>

namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace LinkProto {

	struct ReqStreamData {
		void *data;
		uint32_t size;
	};

	struct AckStreamData {
		void *data;
		uint32_t size;
	};

	struct ReqLinkData {
		uint32_t protoId;
	};
	
	struct AckLinkData {
		uint32_t protoId;
	};


}

}

#endif
