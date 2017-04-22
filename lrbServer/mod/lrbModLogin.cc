#include "lrbModLogin.h"
#include "lrbGameProto.h"
#include "lrbGameProtoFunc.h"
#include "lrbNetWork.h"


using namespace lrb::server::mod;
using namespace lrb::GameProto;

extern ReqRoleId g_lrb_GameProto_ReqRoleId;


void ModLogin::initModLogin()
{
	lrb::GameProto::bindReqLoginFunc(std::bind(ModLogin::reqLogin, std::placeholders::_1));

}

void ModLogin::reqLogin(lrb::NetWork::DataPacker *packer)
{
	packer->roleLogin(g_lrb_GameProto_ReqRoleId.roleId);
	lrb::GameProto::packAckLogin(packer);
	packer->addValue();
}

