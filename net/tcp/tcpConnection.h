#ifndef __LRB_TCPCONNECTION_H
#define __LRB_TCPCONNECTION_H

#include "tcpConfig.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

namespace lrb {

		class tcpConnection {
			
				public:
				tcpConnection() : _socketfd(-1), _connected(false) {};
				~tcpConnection() {};

				private:
				int _socketfd;
				struct sockaddr_in _servaddr;
				bool _connected;
				pthread_t _readThread;
				pthread_t _writeThread;
				char _recvline[1024];

				public:
				const char *host();
				const int port();
				void Connect(const char *host, const int port);
				void reConnect();
				void disConnect();
				void configConnect(const char *host, const int port);
				bool isConnected();

				private:
				static void *readData(void *arg);

		};
}



#endif
