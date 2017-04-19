#include "lrbServerNet.h"
#include "lrbModRoleInfo.h"
#include "lrbModChat.h"
#include "lrbNetWork.h"
#include "lrbModLogin.h"


using namespace lrb::server;


void NetWork::initNetWork()
{
	mod::ModRoleInfo::initModRoleInfo();
	mod::ModChat::initModChat();
	mod::ModLogin::initModLogin();
	
	lrb::NetWork::startService(LRB_SERVER_PORT);
}

