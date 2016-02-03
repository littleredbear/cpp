
#include "lrbSocket.h"

using namespace lrb;

void lrbSocket::throwSocketExp(const char *pref, const char *host, const char *port, char errNO) throw (lrbException &)
{	
	std::string msg = pref;
	msg.append(" host:");
	msg.append(host);
	msg.append(" port:");
	msg.append(port);	
	throw lrbException(msg, 1, errNO);

}

void lrbSocket::tcpConnect(const char *host, const char *port) throw (lrbException &)
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

	if (cur == NULL)
		this->throwSocketExp("tcp connect error! no connection!", host, port, 1);

	freeaddrinfo(res);
}

void lrbSocket::tcpListen(const char *host, const char *port)
{
	

}
