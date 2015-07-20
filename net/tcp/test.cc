
#include "tcpConfig.h"
#include <stdio.h>
#include <string.h>
#include "tcpConnection.h"

using namespace lrb;
int main(int argc, char **argv) 
{

		tcpConfig *config = tcpConfig::defaultConfig();	
		printf("%ld\n", (long)config);
		printf("%d, %d\n", config->retry(), config->maxRetry());

		tcpConnection *connection = new tcpConnection();
		connection->Connect("127.0.0.1", 8888);
		const char *buff = "GET /index.html HTTP/1.1<CR><LF>Host: localhost";
		connection->writeData(buff,strlen(buff));
		
		char buff1[1024];
		while (fgets(buff1, 1024, stdin)) {

			fputs(buff1, stdout);
		}

		delete config;
		delete connection;
}

