#include "lrbServerNet.h"
#include "lrbModRoleInfo.h"
#include "lrbModChat.h"
#include "lrbNetWork.h"


using namespace lrb::server;


void NetWork::initNetWork()
{
	mod::ModRoleInfo::initModRoleInfo();
	mod::ModChat::initModChat();
	
	lrb::NetWork::startService(8080);
}

