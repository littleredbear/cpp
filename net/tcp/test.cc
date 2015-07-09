
#include "tcpConfig.h"
#include <stdio.h>
#include "tcpConnection.h"

using namespace lrb;
int main(int argc, char **argv) 
{

		tcpConfig *config = tcpConfig::defaultConfig();	
		printf("%ld\n", (long)config);
		printf("%d, %d\n", config->retry(), config->maxRetry());

		tcpConnection *connection = new tcpConnection();
		connection->Connect("127.0.0.1", 8080);

		delete config;
		delete connection;
}

