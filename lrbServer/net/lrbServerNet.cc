#include "lrbServerNet.h"
#include "lrbModRoleInfo.h"
#include "lrbNetWork.h"


using namespace lrb::server;


void NetWork::initNetWork()
{
	mod::ModRoleInfo::initModRoleInfo();
	
	lrb::NetWork::startService(8080);
}

