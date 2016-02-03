#ifndef _LRB_SOCKET_H
#define _LRB_SOCKET_H

#include "lrbLog.h"
#include "lrbException.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>


#define MAX_SOCK_BUFF_LEN 1024
namespace lrb {

	class lrbSocket {
	
		private:	
			int _fd;
			char _buff[MAX_SOCK_BUFF_LEN];
			std::string _host;
			std::string _port;
			void throwSocketExp(const char *pref, const char *host, const char *port, char errNO) throw (lrbException &);

		public:
			lrbSocket():_fd(-1){};
			~lrbSocket() {};
			void tcpConnect(const char *host, const char *port) throw (lrbException &);
			void tcpListen(const char *host, const char *port);
	};

}


#endif
