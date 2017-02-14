#include "lrbNetWork.h"
#include "lrbRunLoop.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <assert.h>
#include <poll.h>


using namespace lrb::NetWork;


namespace {
	const static int s_maxLinkNum = (1 << 2);
	const static int s_netBuffSize = (1 << 10);

	NetLink s_links[s_maxLinkNum];

	LinkAcceptor s_acceptor;
	LinkManager s_manager;
}

//-------------------------------Net Data-----------------------------

NetData::NetData():
m_verify(0),
m_data(NULL),
m_size(0),
m_next(NULL)
{

}

NetData::~NetData()
{

}

bool NetData::setNetData(int verify, void *data, size_t size)
{
	if (m_size != 0 ||
	    data == NULL ||
	    size <= 0)	
		return false;

	m_verify = verify;
	m_data = data;
	m_size = size;
		
	return true;
}

bool NetData::writeNetData(int sockfd, int verify, int &off)
{
	if (m_size == 0 || m_data == NULL)
		return false;

	if (verify == m_verify)
	{
		int left = m_size - off;

		int ret = write(sockfd, (char *)m_data + off, left);
		if (ret == -1)
		{
			if (errno == EAGAIN ||
			    errno == EINTR)
				return false;

		} else if (ret < left)
		{
			off += ret;
			return false;
		}
	}

	free(m_data);
	off = 0;
	m_size = 0;

	return true;
}

bool NetData::empty()
{
	return m_size == 0;
}

void NetData::bindNextData(NetData *next)
{
	m_next = next;
}

NetData *NetData::nextData()
{
	if (m_next != NULL)
		return m_next;
	
	return this + 1;
}

//---------------------------------------------Net Link---------------------------------

NetLink::NetLink():
m_size(s_defaultNum),
m_state(LinkState::LS_CLOSED),
m_off(0),
m_fd(0),
m_verify(0),
m_handler(0),
m_last(NULL),
m_next(NULL),
m_events(0)
{
	m_datas[s_defaultNum - 1].bindNextData(&m_datas[0]);
	m_addData = &m_datas[s_defaultNum - 1];
	m_execData = &m_datas[0];
}

NetLink::~NetLink()
{
	for (auto p : m_ptrs)
		free(p);
}

void NetLink::addNetData(int verify, void *data, size_t size)
{
	if (m_state == LinkState::LS_CLOSED || verify != m_verify)
	{
		free(data);
		return;
	}
	
	if (m_addData->nextData()->setNetData(verify, data, size) == false)
	{
		NetData *ptr = (NetData *)calloc(m_size, sizeof(NetData));
		if (ptr == NULL)
			return;

		ptr[m_size-1].bindNextData(m_addData->nextData());
		m_addData->bindNextData(ptr);
		ptr->setNetData(verify, data, size);
		m_addData = ptr;
		
		m_ptrs.push_back(ptr);
		m_size = m_size << 1;
		
	} else {
		m_addData = m_addData->nextData();
	}

	if (!(m_events & POLLOUT))
	{
		m_events = POLLIN | POLLOUT;
		RunLoop::updatePollFd(m_handler, m_events, std::function<void(int, short)>());
	}
	
}

void NetLink::disConnect()
{
	if (m_state == LinkState::LS_CLOSED)
		return;

	RunLoop::removePollFd(m_handler);
	close(m_fd);
	m_state = LinkState::LS_CLOSED;

#ifdef LRB_APPSERVER
	s_manager.reuseNetLink(this);
#endif
	//close callback to be continued
}

void NetLink::connectServer(const std::string &host, const std::string &service)
{
	m_host = host;
	m_service = service;
	
	struct addrinfo hints;
	struct addrinfo *res, *cur;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	
	int ret = getaddrinfo(host.c_str(), service.c_str(), NULL, &res);
	if (ret == -1) 
		return; // fail to be continued

	for (cur = res;cur != NULL; cur = cur->ai_next)
	{
		int sockfd = socket(cur->ai_family, cur->ai_socktype, 0);
		if (sockfd == -1)
			continue;
		
		int flags = fcntl(sockfd, F_GETFL);
		assert(flags != -1);
		assert(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == 0);

		assert(connect(sockfd, cur->ai_addr, cur->ai_addrlen) == -1);
		
		if (errno == EINPROGRESS)
		{
			m_fd = sockfd;
			m_events = POLLIN | POLLOUT;
			++m_verify;
			m_state = LinkState::LS_TOLINK;

			m_handler = RunLoop::addPollFd(sockfd, m_events, std::bind(&NetLink::linkFunc, this, std::placeholders::_1, std::placeholders::_2));
			return;
		}

		close(sockfd);
	}

	// failed to be continued
}

void NetLink::acceptLink(int sockfd)
{
	m_fd = sockfd;
	m_events = POLLIN;
	++m_verify;
	m_state = LinkState::LS_LINKED;
	
	m_handler = RunLoop::addPollFd(sockfd, m_events, std::bind(&NetLink::linkFunc, this, std::placeholders::_1, std::placeholders::_2));
}

void NetLink::bindLastLink(NetLink *link)
{
	m_last = link;
}

void NetLink::bindNextLink(NetLink *link)
{
	m_next = link;
}

NetLink *NetLink::lastLink()
{
	if (m_last == NULL)
		return this - 1;
	return m_last;
}

NetLink *NetLink::nextLink()
{
	if (m_next == NULL)
		return this + 1;
	return m_next;
}

void NetLink::sendNetData()
{
	if (m_state == LinkState::LS_TOLINK)
	{
		m_state = LinkState::LS_LINKED;
		//connection established
	}

	while(m_execData->writeNetData(m_fd, m_verify, m_off))
		m_execData = m_execData->nextData();

}

void NetLink::readNetData()
{
	do {
		char *buff = (char *)calloc(s_netBuffSize, sizeof(char));
		if (buff == NULL)
			return;
		
		int ret = read(m_fd, buff, s_netBuffSize);
		if (ret > 0)
		{
			RunLoop::runInLoop(std::bind(&lrb::NetData::DataParser::parseNetData, &m_parser, buff, ret, m_verify, this), RunLoopType::RLT_LOGIC);
		} else 
		{
			free(buff);
			break;
		}

	}while(1);

}

void NetLink::linkFunc(int sockfd, short events)
{
	if (events & POLLHUP)
	{
		disConnect();
		return;
	}

	if (events & POLLIN)
	{
		readNetData();
	}

	if (events & POLLOUT)
	{
		sendNetData();
	}
}

//----------------------------------------------Link Manager------------------------------

LinkManager::LinkManager():
m_size(s_defaultNum)
{
	m_head = m_links;
	m_able = m_head;
	m_back = m_links + m_size - 1;
}

LinkManager::~LinkManager()
{

}

NetLink *LinkManager::getAvailableLink()
{
	if (m_able == m_back)
	{
		NetLink *ptr = (NetLink *)calloc(m_size, sizeof(NetLink));
		if (ptr == NULL)
			return NULL;
		
		m_back->bindNextLink(ptr);
		ptr->bindLastLink(m_back);
		m_back = ptr + m_size - 1;
		m_size = m_size << 1;
	}

	NetLink *last = m_able;
	m_able = m_able->nextLink();
	return last;
}

void LinkManager::reuseNetLink(NetLink *link)
{
	NetLink *next = link->nextLink();
	if (next == m_able)
	{
		m_able = link;
		return;
	}

	if (m_head == link)
	{	
		m_head = next;
	} else 
	{
		NetLink *last = link->lastLink();
		last->bindNextLink(next);
		next->bindLastLink(last);
	}

	m_back->bindNextLink(link);
	link->bindLastLink(m_back);
	m_back = link;
	
}

//---------------------------------------------Link Acceptor-----------------------------

void LinkAcceptor::startService(short service)
{
	int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (sockfd == -1)
		return; // fail to be continued

	int flags = fcntl(sockfd, F_GETFL);
	assert(flags != -1);
	assert(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == 0);

	struct sockaddr_in6 servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_port = htons(service);
	
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
		return; // fail to be continued

	if (listen(sockfd, s_maxLinkNum) == -1)
		return; // fail to be continued
	
	RunLoop::addPollFd(sockfd, POLLIN, std::bind(&LinkAcceptor::acceptFunc, this, std::placeholders::_1, std::placeholders::_2));
}

void LinkAcceptor::acceptFunc(int fd, short events)
{
	if (events & POLLIN)
	{
		struct sockaddr_in6 cliaddr;
		socklen_t clilen = sizeof(cliaddr);
		do {	
			int sockfd = accept(fd, (struct sockaddr *)&cliaddr, &clilen);
			if (sockfd == -1)
				break;
			
			int flags = fcntl(sockfd, F_GETFL);
			assert(flags != -1);
			assert(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == 0);
			
			NetLink *link = s_manager.getAvailableLink();
			if (link == NULL)
				break; // to be continued 

			link->acceptLink(sockfd);
	
		} while(1);
	}
}


namespace lrb {

namespace NetWork {

void connectServer(const std::string &hostname, const std::string &service, int uuid)
{
	RunLoop::runInLoop(std::bind(&NetLink::connectServer, &s_links[uuid], hostname, service), RunLoopType::RLT_NET);
}

void startService(short service)
{
	RunLoop::runInLoop(std::bind(&LinkAcceptor::startService, &s_acceptor, service), RunLoopType::RLT_NET);
}

void disConnect(int uuid)
{
	RunLoop::runInLoop(std::bind(&NetLink::disConnect, &s_links[uuid]), RunLoopType::RLT_NET);
}

void sendData(int uuid, int verify, void *data, size_t size)
{
	RunLoop::runInLoop(std::bind(&NetLink::addNetData, &s_links[uuid], verify, data, size), RunLoopType::RLT_NET);
}

}

}



