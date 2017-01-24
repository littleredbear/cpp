#include "lrbNetWork.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


using namespace lrb;

namespace {

	DataManager s_dataManager[RunLoopType::RLT_TOP-3];
	LinkData s_linkData[s_makLinkNum];
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




//--------------------------------------------Net Task---------------------------------


//---------------------------------------------Net Link---------------------------------


//----------------------------------------------Link Data---------------------------------

LinkData::LinkData():
m_link(NULL),
m_verify(0)
{

}

LinkData::~LinkData()
{

}

void LinkData::setNetLink(NetLink *link)
{
	m_link = link;
}

NetLink *LinkData::getNetLink()
{
	return m_link;
}

void LinkData::nextVerify()
{
	++m_verify;
}

int LinkData::getVerify()
{
	return m_verify;
}

//----------------------------------------------Link Manager------------------------------





