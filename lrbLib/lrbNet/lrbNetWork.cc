#include "lrbNetWork.h"
#include "lrbRunLoop.h"
#include "lrbLinkProto.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <assert.h>
#include <poll.h>
#include <arpa/inet.h>


using namespace lrb::NetWork;

namespace lrb {

namespace LinkProto {
	void *getUnpackDest(int protoId);
	short *getProtoConfs(int protoId);

}

namespace GameProto {
	void *getUnpackDest(int protoId);
	short *getProtoConfs(int protoId);
	void execReqFunc(int protoId, DataPacker *packer);
	void execAckFunc();
}

}
	
extern lrb::LinkProto::ReqLinkData g_lrb_LinkProto_ReqLinkData;
extern lrb::LinkProto::AckLinkData g_lrb_LinkProto_AckLinkData;

namespace {

	const static int s_maxLinkNum = (1 << 2);
	const static int s_netBuffSize = (1 << 10);

	char s_netCache[s_netBuffSize];

	NetLink s_links[s_maxLinkNum];

	LinkAcceptor s_acceptor;
	LinkManager s_manager;

	DataCenter s_center;

	std::function<void(NetLink *)> s_linkConnectFunc;
	std::function<void(NetLink *, int)> s_lrb_linkProtoFunc;
	std::function<void()> s_lrb_finalCheckFunc;

	void *(*s_lrb_proto_unpackdests[])(int) = {
		lrb::LinkProto::getUnpackDest,
		lrb::GameProto::getUnpackDest,
	};

	short *(*s_lrb_proto_confs[])(int) = {
		lrb::LinkProto::getProtoConfs,
		lrb::GameProto::getProtoConfs,
	};

	void (*s_lrb_proto_reqfuncs[])(int, DataPacker *) = {
		NULL,
		lrb::GameProto::execReqFunc,
	};

	void (*s_lrb_proto_ackfuncs[])() = {
		NULL,
		lrb::GameProto::execAckFunc,
	};

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

void DataPacker::packData(void *data, int protoId, ProtoType type)
{
        ++ m_curVal;

	if (data == NULL)
		return;
		
	void *res;
	int ret = ::packData(data, protoId, &res, type);

	if (ret == -1)
		return;

        m_datas.push_back(res);
        m_lens.push_back(ret);

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

void DataPacker::sendData(int linkId)
{
	void *data;
	int ret = getData(&data);

	::sendData(linkId, data, ret);
}

int DataPacker::getData(void **res)
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
	
	*res = data;
	return size + sizeof(int);
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
	void *data;
	int ret = getData(&data);

        RunLoop::runInLoop(std::bind(&NetLink::addNetData, m_link, m_verify, data, ret), RunLoopType::RLT_NET);
        reusePacker();
}

void DataPacker::reusePacker()
{
        m_datas.clear();
        m_lens.clear();
        m_curVal = 0;
        m_doneVal = 0;

		RunLoop::runInLoop(std::bind(&DataCenter::reusePacker, &s_center, this), RunLoopType::RLT_NET);
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
//                DataPacker *ptr = (DataPacker *)calloc(m_size, sizeof(DataPacker));
		DataPacker *ptr = new DataPacker[m_size];
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
	ProtoType ptype = link->currentProtoType();
	if (ptype < ProtoType::PT_LINK || ptype >= ProtoType::PT_TOP)
		return;

	int val = 0;
	TerminalType ttype = link->currentTType();
	DataPacker *packer = ptype != ProtoType::PT_LINK && ttype == TerminalType::TT_SERVER ? s_center.getAvailablePacker() : NULL;

	while(len > 0)
	{
		int plen;
		memcpy(&plen, frame, sizeof(int));
		frame += sizeof(int);
		len -= sizeof(int);

		int uuid = unpackData(frame, plen, ptype);
		frame += plen;
		len -= plen;
		
		if (uuid == -1)
			continue;

		if (ptype == ProtoType::PT_LINK)
			link->processLinkProto(uuid);
		else if (ttype == TerminalType::TT_SERVER)
		{
			RunLoop::runInLoop(std::bind(s_lrb_proto_reqfuncs[(int)ptype], uuid, packer), RunLoopType::RLT_LOGIC);
			++val;
		}
	}
	
	if (ptype == ProtoType::PT_LINK)
		return;

	if (ttype == TerminalType::TT_SERVER)
	{
		RunLoop::runInLoop(s_lrb_finalCheckFunc, RunLoopType::RLT_LOGIC);
		RunLoop::runInLoop(std::bind(&DataPacker::setDoneValue, packer, val, verify, link), RunLoopType::RLT_LOGIC);
	} else if (ttype == TerminalType::TT_CLIENT)
	{
		RunLoop::runInLoop(std::bind(s_lrb_proto_ackfuncs[(int)ptype]), RunLoopType::RLT_LOGIC);
	}

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
m_ttype(TerminalType::TT_NONE),
m_protoType(ProtoType::PT_LINK),
m_off(0),
m_verify(0),
m_tcpHandler(-1),
m_udpHandler(-1),
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
	if (verify == -1)
		verify = m_verify;

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
		RunLoop::updatePollFd(m_tcpHandler, m_events, std::function<void(int, short)>());
	}
	
}

void NetLink::disConnect()
{
	if (m_state == LinkState::LS_CLOSED)
		return;

	bool reuse = m_ttype == TerminalType::TT_SERVER;
	RunLoop::removePollFd(m_tcpHandler);
	RunLoop::removePollFd(m_udpHandler);
	m_ttype = TerminalType::TT_NONE;
	m_tcpHandler = -1;
	m_udpHandler = -1;
	setProtoType(ProtoType::PT_LINK);
	m_state = LinkState::LS_CLOSED;

	if (reuse)
		s_manager.reuseNetLink(this);
	//close callback to be continued
}

void NetLink::connectServer(const std::string &host, const std::string &service)
{
	if (m_tcpHandler != -1)
		return;

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
			m_events = POLLIN | POLLOUT;
			++m_verify;
			m_ttype = TerminalType::TT_CLIENT;
			m_state = LinkState::LS_TOLINK;

			m_tcpHandler = RunLoop::addPollFd(sockfd, m_events, std::bind(&NetLink::linkFunc, this, std::placeholders::_1, std::placeholders::_2));
			return;
		}

		close(sockfd);
	}

	// failed to be continued
}

void NetLink::mcastJoinGroup(const std::string &group, const std::string &source, short port, int family)
{
	if (m_udpHandler != -1)
		return;

	int sockfd = socket(family, SOCK_DGRAM, 0);
	if (sockfd == -1)
		return;

	int flags = fcntl(sockfd, F_GETFL);
	assert(flags != -1);
	assert(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == 0);

	if (family == AF_INET)
	{
		mcastJoinIPV4Group(sockfd, group, source, port);
	}

}

void NetLink::acceptLink(int sockfd)
{
	m_events = POLLIN;
	++m_verify;
	m_ttype = TerminalType::TT_SERVER;
	m_state = LinkState::LS_LINKED;
	
	m_tcpHandler = RunLoop::addPollFd(sockfd, m_events, std::bind(&NetLink::linkFunc, this, std::placeholders::_1, std::placeholders::_2));
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

TerminalType NetLink::currentTType()
{
	return m_ttype;
}

ProtoType NetLink::currentProtoType()
{
	return m_protoType;
}

void NetLink::setProtoType(ProtoType type)
{
	m_protoType = type;
}

void NetLink::processLinkProto(int protoId)
{
	switch (protoId)
	{
		case 2:
			processReqLinkData();
			break;

		case 3:
			processAckLinkData();
			break;

		default:
			break;
	}

	if (s_lrb_linkProtoFunc)
		RunLoop::runInLoop(std::bind(s_lrb_linkProtoFunc, this, protoId), RunLoopType::RLT_LOGIC);

}

void NetLink::mcastJoinIPV4Group(int sockfd, const std::string &group, const std::string &source, short port)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		return;

	struct ip_mreq_source mreq;
        inet_pton(AF_INET, group.c_str(), &mreq.imr_multiaddr);
	inet_pton(AF_INET, source.c_str(), &mreq.imr_sourceaddr);
        mreq.imr_interface.s_addr = INADDR_ANY;

	if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, &mreq, sizeof(mreq)) != 0)
		return;

	m_udpHandler = RunLoop::addPollFd(sockfd, POLLIN, std::bind(&NetLink::linkFunc, this, std::placeholders::_1, std::placeholders::_2));

}

void NetLink::sendNetData(int sockfd)
{
	if (m_state == LinkState::LS_TOLINK)
	{
		m_state = LinkState::LS_LINKED;
		if (s_linkConnectFunc)
			RunLoop::runInLoop(std::bind(s_linkConnectFunc, this), RunLoopType::RLT_LOGIC);
		//connection established
	}

	while(m_execData->writeNetData(sockfd, m_verify, m_off))
		m_execData = m_execData->nextData();

	if (m_execData->empty() && (m_events & POLLOUT)) {
		m_events = POLLIN;
		RunLoop::updatePollFd(m_tcpHandler, m_events, std::function<void(int, short)>());
	}
}

void NetLink::readNetData(int sockfd)
{
	do {
		int ret = read(sockfd, s_netCache, s_netBuffSize);
		if (ret > 0)
		{
			m_parser.parseNetData(s_netCache, ret, m_verify, this);
		} else 
		{
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
		readNetData(sockfd);
	}

	if (events & POLLOUT)
	{
		sendNetData(sockfd);
	}
}

void NetLink::processReqLinkData()
{
	ProtoType ptype = (ProtoType)g_lrb_LinkProto_ReqLinkData.protoType;
	if (ptype <= ProtoType::PT_LINK || ptype >= ProtoType::PT_TOP)
		ptype = ProtoType::PT_LINK;

	setProtoType(ptype);
	
	lrb::LinkProto::AckLinkData ldata;
	ldata.protoType = (int)ptype;

	DataPacker packer;
	packer.packData(&ldata, 3, ProtoType::PT_LINK);

	void *res;
	int ret = packer.getData(&res);
	
	RunLoop::runInLoop(std::bind(&NetLink::addNetData, this, m_verify, res, ret), RunLoopType::RLT_NET);
}

void NetLink::processAckLinkData()
{
	ProtoType ptype = (ProtoType)g_lrb_LinkProto_AckLinkData.protoType;
	if (ptype <= ProtoType::PT_LINK || ptype >= ProtoType::PT_TOP)
		ptype = ProtoType::PT_LINK;

	setProtoType(ptype);

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

void mcastJoinGroup(const std::string &group, const std::string &source, short port, int uuid)
{
	RunLoop::runInLoop(std::bind(&NetLink::mcastJoinGroup, &s_links[uuid], group, source, port, AF_INET), RunLoopType::RLT_NET);
}

void startService(short service)
{
	RunLoop::runInLoop(std::bind(&LinkAcceptor::startService, &s_acceptor, service), RunLoopType::RLT_NET);
}

void disConnect(int uuid)
{
	RunLoop::runInLoop(std::bind(&NetLink::disConnect, &s_links[uuid]), RunLoopType::RLT_NET);
}

void sendData(int uuid, void *data, size_t size)
{
	RunLoop::runInLoop(std::bind(&NetLink::addNetData, &s_links[uuid], -1, data, size), RunLoopType::RLT_NET);
}

int packData(const void *data, int uuid, void **res, ProtoType ptype)
{
		if (ptype < ProtoType::PT_LINK || ptype >= ProtoType::PT_TOP)
			return -1;

		if (uuid == 0 || uuid == 1)
		{
			int32_t size;
			memcpy(&size, (char *)data + sizeof(void *), sizeof(int32_t));
			if (size <= 0)
				return -1;

			char *ptr = (char *)calloc(size + sizeof(int32_t), sizeof(char));
			if (ptr == NULL)
				return -1;

			memcpy(ptr, &size, sizeof(int32_t));
			memcpy(ptr + sizeof(int32_t), *(void **)data, size);

			return size + sizeof(int32_t);
		}

		short *lrb_proto_confs = s_lrb_proto_confs[(int)ptype](uuid);
		if (lrb_proto_confs == NULL)
			return -1;

        int len = 5;
        for (int i=1;i<5;++i)
        {
                if (lrb_proto_confs[i] > 0)
                        len += (lrb_proto_confs[i] + 3);
        }

        int c = 0;
		const char *src = (const char *)data;
        while(c < lrb_proto_confs[0])
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
                if (lrb_proto_confs[i] > 0)
                {
                        offs[i] = off;
                        off += lrb_proto_confs[i];
                }
        }

        for (char i=1;i<5;++i)
        {
                short vlen = lrb_proto_confs[i];
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

        ptr += sizeof(char); // 字符串类型0
        c = 0;
        while(c < lrb_proto_confs[0])
        {
                std::string *sptr = (std::string *)(src + c);
                memcpy(ptr, sptr->c_str(), sptr->size());
                ptr += (sptr->size() + 1);

                c += sizeof(std::string);
        }

        return len;
}

int unpackData(const char *src, int size, ProtoType ptype)
{
	if (ptype < ProtoType::PT_LINK || ptype >= ProtoType::PT_TOP)
		return -1;
	
		int uuid;
        memcpy(&uuid, src, sizeof(int));
        src += sizeof(int);
        size -= sizeof(int);

		char *dst = (char *)s_lrb_proto_unpackdests[(int)ptype](uuid);
		if (dst == NULL)
			return -1;

		if (uuid == 0 || uuid == 1)
		{
			memcpy(dst + sizeof(void *), &size, sizeof(void *));

			void *data = *(void **)dst;
			if (data != NULL)
				free(data);

			void *ptr = calloc(size, sizeof(char));
			memcpy(ptr, src, size);
			memcpy(dst, &ptr, sizeof(void *));

			return uuid;
		}

		short *lrb_proto_confs = s_lrb_proto_confs[(int)ptype](uuid);
		if (lrb_proto_confs == NULL)
			return -1;

        int off = 0;
        short offs[5] = {-1,-1,-1,-1,-1};
        for (int i=0;i<5;++i)
        {
                if (lrb_proto_confs[i] > 0)
                {
                        offs[i] = off;
                        off += lrb_proto_confs[i];
                }
        }

        memset(dst + lrb_proto_confs[0], 0, off-lrb_proto_confs[0]);
        while(size > 0)
        {
                char type = *(char *)src;
                src += sizeof(char);
                size -= sizeof(char);

                if (type == 0)
                {
                        int c = 0;
                        while(c < lrb_proto_confs[0])
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
                                memcpy(dst + offs[type], src, std::min(len, lrb_proto_confs[type]));

                        src += len;
                        size -= len;
                }
        }

        return uuid;
}

void bindConnectFunc(const std::function<void(NetLink *)> &func)
{
	s_linkConnectFunc = func;
}

void bindLinkProtoFunc(const std::function<void(NetLink *, int protoId)> &func)
{
	s_lrb_linkProtoFunc = func;
}

void bindFinalCheckFunc(const std::function<void()> &func)
{
	s_lrb_finalCheckFunc = func;
}

}

}



