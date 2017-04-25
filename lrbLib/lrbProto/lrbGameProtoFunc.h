#ifndef LRB_GameProto_H
#define LRB_GameProto_H

#include "lrbGameProto.h"

namespace lrb {

namespace NetWork {

	class DataPacker;
}

namespace GameProto {

void packReqStreamData(lrb::NetWork::DataPacker *packer, void *data, int32_t size);
void bindReqStreamDataFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckStreamData(lrb::NetWork::DataPacker *packer, void *data, int32_t size);
void packReqAckFuncType(lrb::NetWork::DataPacker *packer, uint32_t acktype);
void bindReqAckFuncTypeFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckAckFuncType(lrb::NetWork::DataPacker *packer, uint32_t acktype);
void packReqVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify);
void bindReqVerifyDataFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify);
void packReqErrorCode(lrb::NetWork::DataPacker *packer);
void bindReqErrorCodeFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckErrorCode(lrb::NetWork::DataPacker *packer, uint32_t errorCode);
void packReqRoleId(lrb::NetWork::DataPacker *packer, uint32_t roleId);
void bindReqRoleIdFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckRoleId(lrb::NetWork::DataPacker *packer, uint32_t roleId);
void packReqRoleName(lrb::NetWork::DataPacker *packer, const std::string& name);
void bindReqRoleNameFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckRoleName(lrb::NetWork::DataPacker *packer, const std::string& name);
void packReqRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy);
void bindReqRolePosFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy);
void packReqLogin(lrb::NetWork::DataPacker *packer);
void bindReqLoginFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckLogin(lrb::NetWork::DataPacker *packer);
void packReqModId(lrb::NetWork::DataPacker *packer, uint32_t modId);
void bindReqModIdFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckModId(lrb::NetWork::DataPacker *packer, uint32_t modId, uint32_t modRoleId);
void packReqFightRoleData(lrb::NetWork::DataPacker *packer);
void bindReqFightRoleDataFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckFightRoleData(lrb::NetWork::DataPacker *packer, uint32_t fightRoleId, uint32_t itemId, uint32_t hp);
void packReqFightAttack(lrb::NetWork::DataPacker *packer);
void bindReqFightAttackFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckFightAttack(lrb::NetWork::DataPacker *packer, uint32_t roleId, uint32_t itemId, uint32_t hp);
void packReqFightDefend(lrb::NetWork::DataPacker *packer);
void bindReqFightDefendFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckFightDefend(lrb::NetWork::DataPacker *packer, uint32_t roleId, uint32_t hp);
void packReqFightBuff(lrb::NetWork::DataPacker *packer);
void bindReqFightBuffFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckFightBuff(lrb::NetWork::DataPacker *packer, uint32_t targetId, uint32_t buffId, uint32_t buffValue, uint32_t endRound);
void packReqFightRound(lrb::NetWork::DataPacker *packer);
void bindReqFightRoundFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckFightRound(lrb::NetWork::DataPacker *packer, uint32_t roundId);
void packReqFight(lrb::NetWork::DataPacker *packer, uint32_t modTargetId);
void bindReqFightFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckFight(lrb::NetWork::DataPacker *packer);
void packReqUseItem(lrb::NetWork::DataPacker *packer, uint32_t itemId, uint32_t userId, uint32_t targetId);
void bindReqUseItemFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckUseItem(lrb::NetWork::DataPacker *packer);
void packReqChatInfo(lrb::NetWork::DataPacker *packer, uint32_t targetId, uint8_t channelId);
void bindReqChatInfoFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckChatInfo(lrb::NetWork::DataPacker *packer, uint32_t targetId, uint8_t channelId);
void bindAckFunc(AckFuncType acktype, const std::function<void()> &func);

}

}

#endif
