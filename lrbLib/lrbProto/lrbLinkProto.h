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
		int32_t protoType;
	};
	
	struct AckLinkData {
		int32_t protoType;
	};


}

}

#endif
