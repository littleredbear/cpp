#ifndef _LRB_GAME_PROTO_H
#define _LRB_GAME_PROTO_H

#include <string>
#include <functional>

//整形必须用int32_t
//结构排序string,8,4,2,1字节

namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace GameProto {

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

	struct ReqRoleInfo {
		uint32_t roleId;
	};

	struct AckRoleInfo {
		uint32_t roleId;
	};

	struct ReqRoleName {
		std::string name;
	};
	
	struct AckRoleName {
		std::string name;
	};

	struct ReqRolePos {
		uint32_t posx;
		uint32_t posy;
	};
	
	struct AckRolePos {
		uint32_t posx;
		uint32_t posy;
	};

	void bindReqFunc(int protoId, const std::function<void(lrb::NetWork::DataPacker *)> &func);
	void bindAckFunc(AckFuncType acktype, const std::function<void()> &func);

}

}


#endif
