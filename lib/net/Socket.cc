
#include "net/Socket.h"
#include "base/Thread.h"

using namespace lrb;

Socket::~Socket()
{
	if (this->_fd != -1)
		close(this->_fd);	

}

void Socket::throwSocketExp(const char *pref, const char *host, const char *port, char errNO) throw (Exception &)
{	
	std::string msg = pref;
	msg.append(" host:");
	msg.append(host);
	msg.append(" port:");
	msg.append(port);	
	throw Exception(msg, 1, errNO);

}

void Socket::tcpConnect(const char *host, const char *port) throw (Exception &)
{
	if (!host || !port || (!this->_host.compare(host) && !this->_port.compare(port)))
		return;

	struct addrinfo hints, *res, *cur;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int n = getaddrinfo(host, port, &hints, &res);
	if (n != 0) {
		this->throwSocketExp("tcp connect error! getaddrinfo error!", host, port, 1);
	}

	cur = res;

	if (this->_fd != -1)
		close(this->_fd);
	do {
		this->_fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
		if (this->_fd < 0)
			continue;

		if (connect(this->_fd, cur->ai_addr, cur->ai_addrlen) == 0)
			break;

		close(this->_fd);

	} while ((cur = cur->ai_next) != NULL);

	if (cur == NULL) {
		freeaddrinfo(res);
		this->throwSocketExp("tcp connect error! no connection!", host, port, 2);
	}

	freeaddrinfo(res);

}

void Socket::tcpListen(const char *host, const char *port, socklen_t *addrlenp) throw (Exception &)
{
	struct addrinfo hints, *res, *cur;		
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int n = getaddrinfo(host, port, &hints, &res);
	if ( n!= 0 ) {
		this->throwSocketExp("tcp Listen error! getaddrinfo error!", host, port, 1);
	} 

	cur = res;

	if (this->_fd != -1)
		close(this->_fd);

	const int on = 1;

	do {
		this->_fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);	
		if (this->_fd < 0)
			continue;

		if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0) {
			freeaddrinfo(res);
			this->throwSocketExp("tcp listen error! setsocketopt error!", host, port, 4);
		}

		if (bind(this->_fd, cur->ai_addr, cur->ai_addrlen) == 0)
			break;

		close(this->_fd);
	} while ((cur = cur->ai_next) != NULL);

	if (cur == NULL) {
		freeaddrinfo(res);
		this->throwSocketExp("tcp listen error! no avaiable address!", host, port, 3);
	}

	if (listen(this->_fd, LISTENQ) != 0) {
		freeaddrinfo(res);
		this->throwSocketExp("tcp listen error! listen error!", host, port, 5);
	}

	if (addrlenp)
		*addrlenp = res->ai_addrlen;

	freeaddrinfo(res);	

}

void Socket::buildDataThread()
{

}


