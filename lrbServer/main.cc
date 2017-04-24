
#include "lrbRunLoop.h"
#include "lrbServerNet.h"
#include "lrbModData.h"

void gameServerStart()
{
	lrb::server::mod::DataCache::getInstance()->initDataCache();
	lrb::server::NetWork::initNetWork();
}

int main(int argc, char **argv)
{
	lrb::RunLoop::initRunLoop(std::bind(gameServerStart));
}
