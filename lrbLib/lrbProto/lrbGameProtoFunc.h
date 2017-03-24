#ifndef LRB_GameProto_H
#define LRB_GameProto_H

#include <string>

namespace lrb {

namespace NetWork {

	class DataPacker;
}

namespace GameProto {

void packReqStreamData(lrb::NetWork::DataPacker *packer, void *data);
void packAckStreamData(lrb::NetWork::DataPacker *packer, void *data);
void packReqVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify);
void packAckVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify);
void packReqRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId);
void packAckRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId);
void packReqRoleName(lrb::NetWork::DataPacker *packer, const std::string& name);
void packAckRoleName(lrb::NetWork::DataPacker *packer, const std::string& name);
void packReqRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy);
void packAckRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy);

}

}

#endif
