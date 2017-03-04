#ifndef _LRB_VERIFY_PROT_H
#define _LRB_VERIFY_PROTO_H


namespace lrb {

namespace VerifyProto {

	struct ReqStreamData {
		void *data;
	};

	struct AckStreamData {
		void *data;
	};

	struct ReqVerifyData {
		uint32_t verify;
	};

	struct AckVerifyData {
		uint32_t verify;
	};

}

}

#endif
