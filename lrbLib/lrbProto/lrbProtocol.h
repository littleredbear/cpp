#ifndef _LRB_PROTOCOL_H
#define _LRB_PROTOCOL_H

#include <functional>

namespace lrb {

namespace NetWork {
	class NetLink;
}

namespace Protocol {

	enum class ProtoType {
		PT_VERIFY,
		PT_GAME,
		PT_TOP,
	};

	void parseProtoFrame(char *frame, int len, int verify, NetWork::NetLink *link);

}

}


#endif
