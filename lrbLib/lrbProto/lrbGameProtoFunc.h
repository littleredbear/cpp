#ifndef LRB_GameProto_H
#define LRB_GameProto_H

#include <string>

namespace lrb {

namespace NetWork {

	class DataPacker;
}

namespace GameProto {

void packReqStreamData(lrb::NetWork::DataPacker *packer, void *data, int32_t size);
void packAckStreamData(lrb::NetWork::DataPacker *packer, void *data, int32_t size);
void packReqVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify);
void packAckVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify);
void packReqErrorCode(lrb::NetWork::DataPacker *packer, uint32_t errorCode);
void packAckErrorCode(lrb::NetWork::DataPacker *packer, uint32_t errorCode);
void packReqRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId);
void packAckRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId);
void packReqRoleName(lrb::NetWork::DataPacker *packer, const std::string& name);
void packAckRoleName(lrb::NetWork::DataPacker *packer, const std::string& name);
void packReqRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy);
void packAckRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy);
void packReqUseItem(lrb::NetWork::DataPacker *packer, uint32_t itemId, uint32_t userId, uint32_t targetId);
void packAckUseItem(lrb::NetWork::DataPacker *packer);

}

}

#endif
