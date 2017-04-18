#ifndef _LRB_GAME_PROTO_H
#define _LRB_GAME_PROTO_H

#include <string>
#include <functional>

//整形必须用int32_t
//结构排序string,8,4,2,1字节
//StreamData结构必须有，且放在开始

namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace GameProto {

	enum class AckFuncType {
		AFT_BOT,
		AFT_ROLEINFO,
		AFT_ROLENAME,
		AFT_ROLEPOS,
		AFT_TOP,
	};
	
	struct ReqStreamData {
		void *data;
		int32_t size;
	};
	
	struct AckStreamData {
		void *data;
		int32_t size;
	};

	struct ReqVerifyData {
		uint32_t verify;
	};
		
	struct AckVerifyData {
		uint32_t verify;
	};
		
	struct ReqErrorCode {
		uint32_t errorCode;
	};
	
	struct AckErrorCode {
		uint32_t errorCode;
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
		
	struct ReqUseItem {
		uint32_t itemId;
		uint32_t targetId;
	};
	
	struct AckUseItem {

	};

	struct ReqChatInfo {
		uint32_t targetId;
		uint8_t channelId;
	};

	struct AckChatInfo {
		uint32_t targetId;
		uint8_t channelId;
	};

}

}


#endif
