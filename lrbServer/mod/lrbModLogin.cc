#include "lrbModLogin.h"
#include "lrbGameProto.h"
#include "lrbGameProtoFunc.h"
#include "lrbNetWork.h"
#include "lrbModData.h"
#include "lrbErrCode.h"


using namespace lrb::server::mod;
using namespace lrb::GameProto;


extern ReqRoleId g_lrb_GameProto_ReqRoleId;


void ModLogin::initModLogin()
{
	lrb::GameProto::bindReqLoginFunc(std::bind(ModLogin::reqLogin, std::placeholders::_1));

}

void ModLogin::reqLogin(lrb::NetWork::DataPacker *packer)
{
	RoleData *rdata = DataCache::getInstance()->getRoleData(g_lrb_GameProto_ReqRoleId.roleId);

	if (rdata == NULL)
	{
		lrb::server::sendErrCode(packer, ErrCode::Err_RoleId);
		return;
	}

	packer->roleLogin(g_lrb_GameProto_ReqRoleId.roleId);

	lrb::GameProto::packAckVerifyData(packer, rdata->getModRoleInfo()->nextVerify());
	lrb::GameProto::packAckLogin(packer);

	packer->addValue();

}

