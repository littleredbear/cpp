
#include "lrbRunLoop.h"
#include "lrbServerNet.h"
#include "lrbServerData.h"

void gameServerStart()
{
	lrb::server::data::DataCache::getInstance()->initDataCache();
	lrb::server::NetWork::initNetWork();
}

int main(int argc, char **argv)
{
	lrb::RunLoop::initRunLoop(std::bind(gameServerStart));
}
