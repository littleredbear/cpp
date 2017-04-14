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
void packReqVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify);
void bindReqVerifyDataFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify);
void packReqErrorCode(lrb::NetWork::DataPacker *packer, uint32_t errorCode);
void bindReqErrorCodeFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckErrorCode(lrb::NetWork::DataPacker *packer, uint32_t errorCode);
void packReqRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId);
void bindReqRoleInfoFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId);
void packReqRoleName(lrb::NetWork::DataPacker *packer, const std::string& name);
void bindReqRoleNameFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckRoleName(lrb::NetWork::DataPacker *packer, const std::string& name);
void packReqRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy);
void bindReqRolePosFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy);
void packReqUseItem(lrb::NetWork::DataPacker *packer, uint32_t itemId, uint32_t userId, uint32_t targetId);
void bindReqUseItemFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckUseItem(lrb::NetWork::DataPacker *packer);
void packReqChatInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId, uint32_t targetId, uint8_t channelId);
void bindReqChatInfoFunc(const std::function<void(lrb::NetWork::DataPacker *)> &func);
void packAckChatInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId, uint32_t targetId, uint8_t channelId);
void bindAckFunc(AckFuncType acktype, const std::function<void()> &func);

}

}

#endif
