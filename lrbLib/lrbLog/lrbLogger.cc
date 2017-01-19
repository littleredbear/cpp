#include "lrbLogger.h"

Logger Logger::s_logger[(int)RunLoopType::RLT_TOP-2];

//---------------------------------Log Cache------------------------------

LogCache::LogCache():
m_state(CacheState::CS_TOADD),
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
	if (left <= 0)
		return 0;
	
	memcpy(m_cache+m_logSize, data, left);
	m_logSize += left;
	
	if (m_logSize >= s_cacheSize)
	{
		m_state = CacheState::CS_TOFLUSH;
		RunLoop::notifyLoop(RunLoopType::RLT_LOG);
	}

	return left;
}

bool LogCache::flush(int fd)
{
	if (m_state != CacheState::CS_TOFLUSH ||
	    m_logSize <= 0)
		return false;

	write(fd, m_cache, m_logSize);
	m_logSize = 0;
	m_state = CacheState::CS_TOADD;

	return true;
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

LogManager::LogManager()
{

}

LogManager::~LogManager()
{

}

void LogManager::addLog(const void *data, size_t size)
{

}

void LogManager::flush()
{

}

void LogManager::flushAll()
{
	LogCache *cache = m_logCache;
	
}


//---------------------------------Logger---------------------------------

void Logger::logData(const void *data, size_t size)
{
	s_logger[(int)RunLoop::currentLoopType()].doLogData(data, size);
}

void Logger::flushAll()
{
	for (int i=0;i<(int)RunLoopType::RLT_TOP-2;++i)
		s_logger[i].flush();
}

Logger::Logger():
m_fp(NULL)
{
	
}

Logger::~Logger()
{

}

void Logger::flush()
{
	fflush(m_fp);
}

