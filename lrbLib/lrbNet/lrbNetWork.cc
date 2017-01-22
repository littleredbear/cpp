#include "lrbNetWork.h"
#include <errno.h>


using namespace lrb;


//-------------------------------Net Task-----------------------------

NetTask::NetTask():
m_data(NULL),
m_size(0),
m_next(NULL)
{

}

NetTask::~NetTask()
{

}

bool NetTask::setNetTask(int fd, const void *data, size_t size)
{
	if (m_size != 0 ||
		data == NULL ||
		size <= 0)	
		return false;

	m_fd = fd;
	m_data = data;
	m_size = size;
		
	return true;
}

bool NetTask::execNetTask()
{
	if (m_size == 0 || m_data == NULL)
		return false;

	int ret = write(fd, m_data, m_size);
	if (ret == -1)
	{
		if (errno == EAGAIN ||
			errno == EINTR)
			return false;
	}

	free(m_data);
	m_size = 0;

	return true;
}

void NetTask::bindNextTask(NetTask *next)
{
	m_next = next;
}

NetTask *NetTask::nextTask()
{
	if (m_next != NULL)
		return m_next;
	
	return this + 1;
}

