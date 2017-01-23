#include "lrbLogger.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "lrbRunLoop.h"

using namespace lrb;

namespace {
	LogManager s_logManager[(int)RunLoopType::RLT_TOP-2];
}
//---------------------------------Log Cache------------------------------

LogCache::LogCache():
m_state(CacheState::CS_TOADD),
m_flushSize(0),
m_logSize(0),
m_next(NULL)
{

}

LogCache::~LogCache()
{

}

size_t LogCache::addLog(const void *data, size_t size)
{
	if (m_state != CacheState::CS_TOADD)
		return 0;

	size_t left = std::min(s_cacheSize - m_logSize, size);
	
	memcpy(m_cache+m_logSize, data, left);
	m_logSize += left;
	
	if (m_logSize >= s_cacheSize)
	{
		m_state = CacheState::CS_TOFLUSH;
		RunLoop::notifyLoop(RunLoopType::RLT_LOG);
	}

	return left;
}

FlushState LogCache::flush(int fd)
{
	if (m_state != CacheState::CS_TOFLUSH ||
	    m_logSize <= 0)
		return FlushState::FS_FAIL;

	size_t left = m_logSize - m_flushSize;
	while(left > 0)
	{
		int ret = write(fd, m_cache + m_flushSize, left);
		if (ret == -1) {
			if (errno == EBADF)
			{
				return FlushState::FS_BADFD;
			}
			return FlushState::FS_FAIL;
		}

		left -= ret;
		m_flushSize += ret;
	}

	m_flushSize = 0;
	m_logSize = 0;
	m_state = CacheState::CS_TOADD;

	return FlushState::FS_SUCCESS;
}

bool LogCache::toFlush()
{
	if (m_state == CacheState::CS_TOFLUSH)
		return true;
	
	if (m_logSize <= 0)
		return false;
	
	m_state = CacheState::CS_TOFLUSH;
	return true;
}

void LogCache::bindNextCache(LogCache *next)
{
	m_next = next;
}

LogCache *LogCache::nextCache()
{
	return m_next;
}

//-------------------------------Log Manager----------------------------

LogManager::LogManager():
m_addCache(NULL),
m_logCache(NULL),
m_fd(-1)
{

}

LogManager::~LogManager()
{
	LogCache *next = m_logCache->nextCache();
	do
	{
		LogCache *curr = next;
		next = next->nextCache();
		free(curr);
	}while(next != m_logCache);

	if (m_fd != -1)
		close(m_fd);
}

void LogManager::initLogFile()
{
	if (m_fd != -1)
		close(m_fd);

	timeval tv;
	gettimeofday(&tv, NULL);
	char buff[128] = {0};
	snprintf(buff, 128, "%s-%ld.log", RunLoop::loopName(), tv.tv_sec);
	m_fd = open(buff, O_WRONLY|O_APPEND|O_CREAT|O_NONBLOCK, 0644);

	if (m_addCache == NULL && m_logCache == NULL)
	{
		LogCache *ptr = (LogCache *)calloc(1, sizeof(LogCache));
		if (ptr == NULL)
			return;
		
		ptr->bindNextCache(ptr);
		m_addCache = ptr;
		m_logCache = ptr;
	}
}

void LogManager::addLog(const void *data, size_t size)
{
	if (m_fd == -1 || size <= 0)
		return;

	const char *dptr = (const char *)data;
	size_t left = size;
	do
	{
		size_t as= m_addCache->nextCache()->addLog(dptr, left);
		if (as == 0)
		{
			LogCache *ptr = (LogCache *)calloc(1, sizeof(LogCache));
			if (ptr == NULL)
				return;

			ptr->bindNextCache(m_addCache->nextCache());
			m_addCache->bindNextCache(ptr);
		} else if (as < left)
		{
			m_addCache = m_addCache->nextCache();
		}

		dptr += as;
		left -= as;
		
	}while(left > 0);
}

void LogManager::flush()
{
	if (m_fd == -1)
		return;

	do
	{
		FlushState state = m_logCache->flush(m_fd);
		if (state == FlushState::FS_SUCCESS)
		{
			m_logCache = m_logCache->nextCache();
			continue;
		}
		
		if (state == FlushState::FS_BADFD)
			initLogFile();
		
		break;

	} while(1);

}

void LogManager::toFlush()
{
	if (m_logCache == NULL)
		return;

	LogCache *cache = m_logCache;
	while(cache->toFlush())
	{
		cache = cache->nextCache();
		if (cache == m_logCache)
			break;
	}
	
	RunLoop::notifyLoop(RunLoopType::RLT_LOG);
}


//---------------------------------Logger---------------------------------

void Logger::logData(const void *data, size_t size)
{
	s_logManager[(int)RunLoop::loopType()].addLog(data, size);
}

void Logger::flush()
{
	for (int i=0;i<(int)RunLoopType::RLT_TOP-2;++i)
		s_logManager[i].flush();
}

void Logger::toFlush()
{
	s_logManager[(int)RunLoopType::loopType()].toFlush();
}

void Logger::initLogger()
{
	s_logManager[(int)RunLoop::loopType()].initLogFile();
}

//Logger::Logger():
//m_fp(NULL)
//{
	
//}

//Logger::~Logger()
//{

//}

//void Logger::flush()
//{
//	fflush(m_fp);
//}

