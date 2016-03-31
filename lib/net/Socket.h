#ifndef _LRB_SOCKET_H
#define _LRB_SOCKET_H

#include "base/Exception.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <list>


#define MAX_SOCK_BUFF_LEN 1024
#define LISTENQ	1024


namespace lrb {

	class Socket {
	
		private:	
			int _fd;
			char _buff[MAX_SOCK_BUFF_LEN];
			std::list<std::string> _sendList;
			std::string _host;
			std::string _port;
			void throwSocketExp(const char *pref, const char *host, const char *port, char errNO) throw (Exception &);
			void recvData();
			void sendData();

		public:
			Socket():_fd(-1) {};
			~Socket();
			void tcpConnect(const char *host, const char *port) throw (Exception &);
			void tcpListen(const char *host, const char *port, socklen_t *addrlenp) throw (Exception &);
			void buildDataThread();
	};

}


#endif
