#include "lrbNetWork.h"
#include "lrbRunLoop.h"
#include "lrbNetData.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>


using namespace lrb::NetData;
using namespace lrb::NetWork;

namespace {
	const static int s_maxLinkNum = (1 << 16);

	DataManager s_dataManager[RunLoopType::RLT_TOP-3];
	NetLink s_links[s_maxLinkNum];
}

//-------------------------------Net Data-----------------------------

NetData::NetData():
m_uuid(0),
m_verify(0),
m_data(NULL),
m_size(0),
m_next(NULL)
{

}

NetData::~NetData()
{

}

bool NetData::setNetData(int uuid, int verify, void *data, size_t size)
{
	if (m_size != 0 ||
	    data == NULL ||
	    size <= 0)	
		return false;

	m_uuid = uuid;
	m_verify = verify;
	m_data = data;
	m_size = size;
		
	return true;
}

bool NetData::writeNetData(int &off)
{
	if (m_size == 0 || m_data == NULL)
		return false;

	int left = m_size - off;

	int ret = write(1, m_data + off, left);
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

//---------------------------------------------Data Manager-------------------------------

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

void DataManager::addNetData(int uuid, void *data, size_t size)
{

}


//--------------------------------------------Net Task---------------------------------


//---------------------------------------------Net Link---------------------------------

NetLink::NetLink():
m_off(0),
m_fd(0),
m_verify(0),
m_handler(0)
{

}

NetLink::~NetLink()
{

}

void NetLink::nextVerify()
{
	++m_verify;
}

int NetLink::getVerify()
{
	return m_verify;
}

void NetLink::disConnect()
{

}

void NetLink::connectServer(const std::string &host, const std::string &service)
{

}

void NetLink::startService(const std::string &service)
{

}


//----------------------------------------------Link Manager------------------------------


void connectServer(const std::string &hostname, const std::string &service, int uuid)
{
	RunLoop::runInLoop(std::bind(&NetLink::connectServer, &s_links[uuid], hostname, service), RunLoopType::RLT_NET);
}

void startService(const std::string &service)
{

}

void disConnect(int uuid)
{
	RunLoop::runInLoop(std::bind(&NetLink::disConnect, &s_links[uuid]), RunLoopType::RLT_NET);
}

void sendData(int uuid, int verify, void *data, size_t size)
{
	s_dataManager[(int)RunLoop::loopType()].addNetData(uuid, data, size);
	RunLoop::runInLoop(std::bind(sortNetData), RunLoopType::RLT_NET);
}

void sortNetData()
{

}




