#include "tcpConnection.h"

using namespace lrb;


bool tcpConnection::isConnected()
{
		return this->_connected;
}

void tcpConnection::Connect(const char *host, const int port)
{

		if (this->isConnected()) {
				this->disConnect();
		}

		this->configConnect(host, port);
		this->reConnect();
}

void tcpConnection::reConnect()
{
		if (this->_socketfd == -1) {
				printf("socket error\n");
				return;
		}

		int c = connect(this->_socketfd, (struct sockaddr *)&this->_servaddr, sizeof(this->_servaddr));
		if (c < 0) {
				printf("connect error\n");
				return;
		}

		pthread_create(&this->_readThread, NULL, tcpConnection::readData, &this->_socketfd);

}

void tcpConnection::disConnect()
{

}

void tcpConnection::configConnect(const char *host, const int port)
{

		this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
		memset(&this->_servaddr, 0, sizeof(this->_servaddr));
		this->_servaddr.sin_family = AF_INET;
		this->_servaddr.sin_port = htons(port);
		inet_pton(AF_INET, host, &this->_servaddr.sin_addr);
}

void tcpConnection::writeData(const char *buff, const int len)
{
		write(this->_socketfd, buff, len);
}

void *tcpConnection::readData(void *arg)
{
		char buff[1024];	
		int sockfd = *((int *)arg);
		int n;
		while ((n = read(sockfd, buff, 1024)) > 0) {
			printf("get msg:%d\n", n);
			fputs(buff, stdout);
		}
		return (void *)0;

}








