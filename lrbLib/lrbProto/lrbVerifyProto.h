#ifndef _LRB_VERIFY_PROTO_H
#define _LRB_VERIFY_PROTO_H

#include <string>
#include <functional>

namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace VerifyProto {

	enum class AckFuncType {
		AFT_BOT,
		AFT_TOP,
	};

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

	void bindReqFunc(int protoId, const std::function<void(lrb::NetWork::DataPacker *)> &func);
	void bindAckFunc(AckFuncType acktype, const std::function<void()> &func);

}

}

#endif
