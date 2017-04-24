#include "lrbModFight.h"
#include "lrbGameProto.h"
#include "lrbGameProtoFunc.h"


using namespace lrb::server::mod;
using namespace lrb::GameProto;


extern ReqFight g_lrb_GameProto_ReqFight;


namespace {

}


void ModFight::initModFight()
{
	lrb::GameProto::bindReqFightFunc(std::bind(ModFight::reqFight, std::placeholders::_1));
}

void ModFight::reqFight(lrb::NetWork::DataPacker *packer)
{
	
}


