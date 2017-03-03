#ifndef _LRB_PROTOCOL_H
#define _LRB_PROTOCOL_H


namespace lrb {

namespace NetWork {
	class NetLink;
}

namespace Protocol {

	class Packer {
	public:
		int packData();
		int unpackData();
	};

	void parseProtoFrame(char *frame, int len, int verify, NetWork::NetLink *link);

	int packGameProtoData(const char *src, int uuid, void **res);
	int unpackGameProtoData(const char *src, int size);

}

}


#endif
