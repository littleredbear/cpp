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
		AFT_CHAT,
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

	struct ReqAckFuncType {
		uint32_t acktype;
	};

	struct AckAckFuncType {
		uint32_t acktype;
	};

	struct ReqVerifyData {
		uint32_t verify;
	};
		
	struct AckVerifyData {
		uint32_t verify;
	};
		
	struct ReqErrorCode {

	};
	
	struct AckErrorCode {
		uint32_t errorCode;
	};

	struct ReqRoleId {
		uint32_t roleId;
	};

	struct AckRoleId {
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

	struct ReqLogin {

	};

	struct AckLogin {

	};

	struct ReqModId {
		uint32_t modId;
	};

	struct AckModId {
		uint32_t modId;
		uint32_t modRoleId;
	};

	struct ReqFightRoleData {

	};

	struct AckFightRoleData {
		uint32_t fightRoleId;
		uint32_t itemId;
		uint32_t hp;
	};

	struct ReqFightAttack {

	};

	struct AckFightAttack {
		uint32_t roleId;
		uint32_t itemId;
		uint32_t hp;
	};

	struct ReqFightDefend {

	};

	struct AckFightDefend {
		uint32_t roleId;
		uint32_t hp;
	};

	struct ReqFightBuff {

	};

	struct AckFightBuff {
		uint32_t targetId;
		uint32_t buffId;
		uint32_t buffValue;
		uint32_t endRound;
	};

	struct ReqFightRound {

	};

	struct AckFightRound {
		uint32_t roundId;
	};

	struct ReqFight {
		uint32_t modTargetId;
	};

	struct AckFight {

	};
		
	struct ReqUseItem {
		uint32_t itemId;
		uint32_t userId;
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
