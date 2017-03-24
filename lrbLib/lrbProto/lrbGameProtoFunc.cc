#include "lrbGameProtoFunc.h"
#include "lrbNetWork.h"
#include "lrbGameProto.h"

using namespace lrb::GameProto;

namespace lrb {

namespace GameProto {

void packReqStreamData(lrb::NetWork::DataPacker *packer, void *data)
{
	ReqStreamData tmpdata;
	tmpdata.data = data;
	packer->packData(&tmpdata, 0, lrb::NetWork::ProtoType::PT_GameProto);
}

void packAckStreamData(lrb::NetWork::DataPacker *packer, void *data)
{
	AckStreamData tmpdata;
	tmpdata.data = data;
	packer->packData(&tmpdata, 1, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify)
{
	ReqVerifyData tmpdata;
	tmpdata.verify = verify;
	packer->packData(&tmpdata, 2, lrb::NetWork::ProtoType::PT_GameProto);
}

void packAckVerifyData(lrb::NetWork::DataPacker *packer, uint32_t verify)
{
	AckVerifyData tmpdata;
	tmpdata.verify = verify;
	packer->packData(&tmpdata, 3, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId)
{
	ReqRoleInfo tmpdata;
	tmpdata.roleId = roleId;
	packer->packData(&tmpdata, 4, lrb::NetWork::ProtoType::PT_GameProto);
}

void packAckRoleInfo(lrb::NetWork::DataPacker *packer, uint32_t roleId)
{
	AckRoleInfo tmpdata;
	tmpdata.roleId = roleId;
	packer->packData(&tmpdata, 5, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqRoleName(lrb::NetWork::DataPacker *packer, const std::string& name)
{
	ReqRoleName tmpdata;
	tmpdata.name = name;
	packer->packData(&tmpdata, 6, lrb::NetWork::ProtoType::PT_GameProto);
}

void packAckRoleName(lrb::NetWork::DataPacker *packer, const std::string& name)
{
	AckRoleName tmpdata;
	tmpdata.name = name;
	packer->packData(&tmpdata, 7, lrb::NetWork::ProtoType::PT_GameProto);
}

void packReqRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy)
{
	ReqRolePos tmpdata;
	tmpdata.posx = posx;
	tmpdata.posy = posy;
	packer->packData(&tmpdata, 8, lrb::NetWork::ProtoType::PT_GameProto);
}

void packAckRolePos(lrb::NetWork::DataPacker *packer, uint32_t posx, uint32_t posy)
{
	AckRolePos tmpdata;
	tmpdata.posx = posx;
	tmpdata.posy = posy;
	packer->packData(&tmpdata, 9, lrb::NetWork::ProtoType::PT_GameProto);
}


}

}

