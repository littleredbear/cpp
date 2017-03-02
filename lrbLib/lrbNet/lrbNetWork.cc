#include "lrbNetWork.h"
#include "lrbRunLoop.h"
#include "lrbProtoBuf.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <assert.h>
#include <poll.h>
#ifndef LRB_APPSERVER
#include <unordered_map>
#endif


using namespace lrb::NetWork;

extern void *g_lrb_protobuf_ptrs[];
extern short g_lrb_protobuf_confs[][5];
extern lrb::ProtoBuf::AckVerifyData g_lrb_protobuf_AckVerifyData;

namespace {

	const static int s_maxLinkNum = (1 << 2);
	const static int s_netBuffSize = (1 << 10);

	NetLink s_links[s_maxLinkNum];

	LinkAcceptor s_acceptor;
	LinkManager s_manager;

	DataCenter s_center;

	std::function<void(NetLink *)> s_linkConnectFunc;

#ifdef LRB_APPSERVER
	std::function<void(DataPacker *)> s_reqFuncs[2];
#else
	std::unordered_map<int, std::function<void()> > s_ackFuncs;
#endif

}

//-----------------------------------------Data Packer--------------------------------------

DataPacker::DataPacker():
m_doneVal(0),
m_curVal(0),
m_last(NULL),
m_next(NULL)
{

}

DataPacker::~DataPacker()
{

}

void DataPacker::packData(void *data, int size, bool verify)
{
        m_datas.push_back(data);
        m_lens.push_back(size);

        if (verify)
        {
                iter_swap(m_datas.begin(), m_datas.end() - 1);
                iter_swap(m_lens.begin(), m_lens.end() - 1);
        }

        ++ m_curVal;
        if (m_doneVal != 0 && m_doneVal == m_curVal)
        {
                sendData();
        }

}

void DataPacker::setDoneValue(int val, int verify, NetLink *link)
{
        m_link = link;
        m_verify = verify;

        if (val == m_curVal)
                sendData();
        else
                m_doneVal = val;
}

void DataPacker::bindLastPacker(DataPacker *packer)
{
        m_last = packer;
}

void DataPacker::bindNextPacker(DataPacker *packer)
{
        m_next = packer;
}

DataPacker *DataPacker::lastPacker()
{
        if (m_last == NULL)
                return this - 1;

        return m_last;
}

DataPacker *DataPacker::nextPacker()
{
        if (m_next == NULL)
                return this + 1;

        return m_next;
}

void DataPacker::sendData()
{
        int size = 0;
        for (auto len : m_lens)
                size += (len + sizeof(int));

        void *data = calloc(size + sizeof(int), sizeof(char));
        char *ptr = (char *)data;

        memcpy(ptr, &size, sizeof(int));
        ptr += sizeof(int);

        for (int i = 0;i < m_datas.size(); ++i)
        {
                int len = m_lens[i];
                memcpy(ptr, &len, sizeof(int));
                ptr += sizeof(int);

                memcpy(ptr, m_datas[i], len);
                ptr += len;
		
		free(m_datas[i]);
        }

        RunLoop::runInLoop(std::bind(&NetLink::addNetData, m_link, m_verify, data, size+sizeof(int)), RunLoopType::RLT_NET);
        reusePacker();
}

void DataPacker::reusePacker()
{
        m_datas.clear();
        m_lens.clear();
        m_curVal = 0;
        m_doneVal = 0;

        s_center.reusePacker(this);
}

//------------------------------------------Data Center-------------------------------------

DataCenter::DataCenter():
m_size(s_defaultNum)
{
        m_head = m_packers;
        m_able = m_head;
        m_back = m_packers + m_size - 1;;
}

DataCenter::~DataCenter()
{

}

DataPacker *DataCenter::getAvailablePacker()
{
        if (m_able == m_back)
        {
                DataPacker *ptr = (DataPacker *)calloc(m_size, sizeof(DataPacker));
                if (ptr == NULL)
                        return NULL; // to be continued

                ptr->bindLastPacker(m_back);
                m_back->bindNextPacker(ptr);
                m_back = ptr + m_size - 1;
                m_size = m_size << 1;
        }

        DataPacker *last = m_able;
        m_able = m_able->nextPacker();

        return last;
}

void DataCenter::reusePacker(DataPacker *packer)
{
        DataPacker *next = packer->nextPacker();
        if (next == m_able)
        {
                m_able = packer;
                return;
        }

        if (packer == m_head)
        {
                m_head = next;
        } else
        {
                DataPacker *last = packer->lastPacker();
                last->bindNextPacker(next);
                next->bindLastPacker(last);
        }

        packer->bindLastPacker(m_back);;
        m_back->bindNextPacker(packer);
        m_back = packer;

}

//------------------------------------------Data Parser---------------------------------------

DataParser::DataParser():
m_dataCache(NULL),
m_cacheLen(0),
m_frameLen(0)
{

}

DataParser::~DataParser()
{

}

void DataParser::parseNetData(char *data, int size, int verify, NetLink *link)
{
        if (m_dataCache == NULL)
        {
                parseFirstData(data, size, verify, link);
        } else
        {
                if (m_verify != verify)
                {
                        free(m_dataCache);
                        m_dataCache = NULL;
                        parseFirstData(data, size, verify, link);
                } else
                {
                        int needLen = m_frameLen - m_cacheLen;
                        if (needLen > size)
                        {
                                memcpy(m_dataCache + m_cacheLen, data, size);
                                m_cacheLen += size;
                        } else
                        {
                                memcpy(m_dataCache + m_cacheLen, data, needLen);
                                parseNetFrame(m_dataCache, m_frameLen, m_verify, link);
                                free(m_dataCache);
                                m_dataCache = NULL;

                                parseFirstData(data + needLen, size-needLen, verify, link);
                        }
                }
        }

        free(data);
}

void DataParser::parseFirstData(char *data, int size, int verify, NetLink *link)
{
        while(size > 0)
        {
                int frameLen;
                memcpy(&frameLen, data, sizeof(int));
                data += sizeof(int);
                size -= sizeof(int);

                if (frameLen > size)
                {
                        m_dataCache = (char *)calloc(frameLen, sizeof(char));
                        if (m_dataCache)
                        {
                                memcpy(m_dataCache, data, size);
                                m_frameLen = frameLen;
                                m_cacheLen = size;
                                m_verify = verify;
                        }
                        break;
                } else
                {
                        parseNetFrame(data, frameLen, verify, link);
                        data += frameLen;
                        size -= frameLen;
                }
        }

}

void DataParser::parseNetFrame(char *frame, int len, int verify, NetLink *link)
{
#ifdef LRB_APPSERVER
        DataPacker *packer = s_center.getAvailablePacker();
        int val = 0;
        while(len > 0)
        {
                int plen;
                memcpy(&plen, frame, sizeof(int));
                frame += sizeof(int);
                len -= sizeof(int);

                int uuid = unpackData(frame, plen);
                frame += plen;
                len -= plen;

                s_reqFuncs[uuid >> 1](packer);
                ++val;
        }
        packer->setDoneValue(val, verify, link);
#else
        while(len > 0)
        {
                int plen;
                memcpy(&plen, frame, sizeof(int));
                frame += sizeof(int);
                len -= sizeof(int);

                int uuid = unpackData(frame, plen);
                frame += plen;
                len -= plen;
        }

        auto iter = s_ackFuncs.find(g_lrb_protobuf_AckVerifyData.verify);
        if (iter != s_ackFuncs.end())
                iter->second();

#endif
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
	
	int ret = getaddrinfo(host.c_str(), service.c_str(), &hints, &res);
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

		int ret = connect(sockfd, cur->ai_addr, cur->ai_addrlen);
		
		if (ret == 0 || errno == EINPROGRESS)
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
		if (s_linkConnectFunc)
			RunLoop::runInLoop(std::bind(s_linkConnectFunc, this), RunLoopType::RLT_LOGIC);
		//connection established
	}

	while(m_execData->writeNetData(m_fd, m_verify, m_off))
		m_execData = m_execData->nextData();

	if (m_execData->empty() && (m_events & POLLOUT)) {
		m_events = POLLIN;
		RunLoop::updatePollFd(m_handler, m_events, std::function<void(int, short)>());
	}
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
			RunLoop::runInLoop(std::bind(&DataParser::parseNetData, &m_parser, buff, ret, m_verify, this), RunLoopType::RLT_LOGIC);
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

int packData(const char *src, int uuid, void **res)
{
        int len = 5;
        for (int i=1;i<5;++i)
        {
                if (g_lrb_protobuf_confs[uuid][i] > 0)
                        len += (g_lrb_protobuf_confs[uuid][i] + 3);
        }

        int c = 0;
        while(c < g_lrb_protobuf_confs[uuid][0])
        {
                std::string *ptr = (std::string *)(src + c);
                len += (ptr->size() + 1);
                c += sizeof(std::string);
        }

        char *ptr = (char *)calloc(len, sizeof(char));
        if (ptr == NULL)
                return -1;

        *res = ptr;
        memcpy(ptr, &uuid, sizeof(int));
        ptr += sizeof(int);

        int off = 0;
        short offs[5] = {-1,-1,-1,-1,-1};
        for (int i=0;i<5;++i)
        {
                if (g_lrb_protobuf_confs[uuid][i] > 0)
                {
                        offs[i] = off;
                        off += g_lrb_protobuf_confs[uuid][i];
                }
        }

        for (char i=1;i<5;++i)
        {
                short vlen = g_lrb_protobuf_confs[uuid][i];
                if (vlen > 0)
                {
                        *ptr = i;
                        ptr += sizeof(char);

                        memcpy(ptr, &vlen, sizeof(short));
                        ptr += sizeof(short);

                        memcpy(ptr, src + offs[i], vlen);
                        ptr += vlen;
                }
        }

        ptr += sizeof(char);
        c = 0;
        while(c < g_lrb_protobuf_confs[uuid][0])
        {
                std::string *sptr = (std::string *)(src + c);
                memcpy(ptr, sptr->c_str(), sptr->size());
                ptr += (sptr->size() + 1);

                c += sizeof(std::string);
        }

        return len;
}

int unpackData(const char *src, int size)
{
        int uuid;
        memcpy(&uuid, src, sizeof(int));
        src += sizeof(int);
        size -= sizeof(int);

        int off = 0;
        short offs[5] = {-1,-1,-1,-1,-1};
        for (int i=0;i<5;++i)
        {
                if (g_lrb_protobuf_confs[uuid][i] > 0)
                {
                        offs[i] = off;
                        off += g_lrb_protobuf_confs[uuid][i];
                }
        }

        char *dst = (char *)g_lrb_protobuf_ptrs[uuid >> 1];
        memset(dst + g_lrb_protobuf_confs[uuid][0], 0, off-g_lrb_protobuf_confs[uuid][0]);

        while(size > 0)
        {
                char type = *(char *)src;
                src += sizeof(char);
                size -= sizeof(char);

                if (type == 0)
                {
                        int c = 0;
                        while(c < g_lrb_protobuf_confs[uuid][0])
                        {
                                std::string *ptr = (std::string *)(dst + c);
                                if (size > 0)
                                        *ptr = src;
                                else
                                        ptr->clear();

                                src += (ptr->size() + 1);
                                size -= (ptr->size() + 1);

                                c += sizeof(std::string);
                        }

                        break;
                } else
                {
                        short len;
                        memcpy(&len, src, sizeof(short));
                        src += sizeof(short);
                        size -= sizeof(short);

                        if (offs[type] != -1)
                                memcpy(dst + offs[type], src, std::min(len, g_lrb_protobuf_confs[uuid][type]));

                        src += len;
                        size -= len;
                }
        }

        return uuid;
}

#ifdef LRB_APPSERVER
void bindReqFunc(int uuid, const std::function<void(DataPacker *)> &func)
{
        s_reqFuncs[uuid >> 1] = func;
}

#else
void bindAckFunc(int verify, const std::function<void()> &func)
{
        s_ackFuncs[verify] = func;
}
#endif

void bindConnectFunc(const std::function<void(NetLink *)> &func)
{
	s_linkConnectFunc = func;
}

}

}



