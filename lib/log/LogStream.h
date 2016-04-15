#ifndef LRB_LOG_LOGSTREAM_H
#define LRB_LOG_LOGSTREAM_H

#include "log/LogBuffer.h"
#include <stdio.h>
#include <cstring>

namespace lrb {

	namespace log {	

		enum LogLevel {
			NONE = 0,
			ERR = 1,
			WARN = 1 << 1,
			INFO = 1 << 2,
			ALL = ERR | WARN | INFO
		};

		class LogStream : public base::noncopyable {
			public:
				LogStream(const char *basename):m_buffer(basename) {m_buffer.start();};

				void flush() {m_buffer.flush();};
	
				LogStream& operator << (const short val) 
				{
					char buff[6];	
					snprintf(buff, 6, "%d", val);
					m_buffer.append(buff, 6);
					return *this;
				};

				LogStream& operator << (const unsigned short val)
				{
					char buff[5];
					snprintf(buff, 5, "%u", val);
					m_buffer.append(buff, 5);
					return *this;
				};

				LogStream& operator << (const int val)
				{
					char buff[11];
					snprintf(buff, 11, "%d", val);
					m_buffer.append(buff, 11);
					return *this;
				};

				LogStream& operator << (const unsigned int val)
				{
					char buff[10];
					snprintf(buff, 10, "%u", val);
					m_buffer.append(buff, 10);
					return *this;
				};

				LogStream& operator << (const long val)
				{
					char buff[20];
					snprintf(buff, 20, "%ld", val);
					m_buffer.append(buff, 20);
					return *this;
				};	

				LogStream& operator << (const unsigned long val)
				{
					char buff[20];
					snprintf(buff, 20, "%lu", val);
					m_buffer.append(buff, 20);
					return *this;
				};

				LogStream& operator << (const long long val)
				{
					char buff[20];
					snprintf(buff, 20, "%lld", val);
					m_buffer.append(buff, 20);
					return *this;
				};

				LogStream& operator << (const unsigned long long val)
				{
					char buff[20];
					snprintf(buff, 20, "%llu", val);
					m_buffer.append(buff, 20);
					return *this;
				};
				
				LogStream& operator << (const char *val)
				{
					m_buffer.append(val, strlen(val));
					return *this;
				};

				LogStream& operator << (const double val)
				{
					char buff[20];
					snprintf(buff, 20, "%.12g", val);
					m_buffer.append(buff, 20);
					return *this;
				};

				template<typename T>
					LogStream& operator << (const T& t)
					{
						m_buffer.append(t.c_str(), t.length());
						return *this;
					};

			private:	
				LogBuffer	m_buffer;
				static LogLevel g_logLevel;
				static void setLogLevel(LogLevel lvl) {g_logLevel = lvl;};
		};
	}

}

extern lrb::log::LogStream g_logStream;

#ifdef LRBLOG

#define LRB_LOG_FLAG 1

#else

#define LRB_LOG_FLAG 0

#endif

#define LOGINFO if(LRB_LOG_FLAG && (lrb::log::g_logLevel & lrb::log::INFO)) lrb::log::g_logStream
#define LOGERR if(LRB_LOG_FLAG && (lrb::log::g_logLevel & lrb::log::ERR)) lrb::log::g_logStream
#define LOGWARN if(LRB_LOG_FLAG && (lrb::log::g_logLevel & lrb::log::WARN)) lrb::log::g_logStream



#endif
