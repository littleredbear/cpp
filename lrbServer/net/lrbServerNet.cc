#include "lrbServerNet.h"
#include "lrbModRoleInfo.h"
#include "lrbNetWork.h"


using namespace lrb::server;


void NetWork::initNetWork()
{
	data::ModRoleInfo::initModRoleInfo();
	
	lrb::NetWork::startService(8080);
}

