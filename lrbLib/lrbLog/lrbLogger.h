#ifndef _LRB_LOGGER_H
#define _LRB_LOGGER_H

#include "lrbRunLoop.h"

namespace lrb {


	enum class CacheState {
		CS_TOADD = 0,
		CS_TOFLUSH
	};

	enum class FlushState {
		FS_SUCCESS,
		FS_BADFD,
		FS_FAIL
	};

//-----------------------Log Cache---------------------------

	class LogCache {
	public:
		const static size_t s_cacheSize = (1 << 17);

		LogCache();
		~LogCache();

		size_t addLog(const void *data, size_t size);
		FlushState flush(int fd);
		bool toFlush();

		void bindNextCache(LogCache *next);
		LogCache *nextCache();

	private:
		CacheState m_state;
		size_t m_logSize;
		LogCache *m_next;
		char m_cache[s_cacheSize];
	};

//--------------------------Log Manager-----------------------

	class LogManager {
	public:
		LogManager();
		~LogManager();
		
		void initLogFile();
		void addLog(const void *data, size_t size);
		void flush();
		void flushAll();

	private:
		LogCache *m_addCache;
		LogCache *m_logCache;
		int m_fd;
	};


//-----------------------------Logger--------------------------------
	
	class Logger {
	public:
		static void logData(const void *data, size_t size);
		static void flush();
		static void flushAll();
		static void initLogger();

//		Logger();
//		~Logger();

//		void flush();

	private:
//		void doLogData(const void *data, size_t size);

//		FILE *m_fp;
		
		static LogManager s_logManager[(int)RunLoopType::RLT_TOP-2];
	};

}


#endif
