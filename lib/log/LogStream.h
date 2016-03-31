#ifndef LRB_LOG_STREAM_H
#define LRB_LOG_STREAM_H

#include "log/LogBuffer.h"
#include <stdio.h>

namespace lrb {

	namespace log {	
		enum LogLevel {
			NONE = 0,
			DEBUG = 1,
			WARN = 1 << 1,
			INFO = 1 << 2
		};

		class LogStream : public noncopyable {
			public:
				LogStream() {m_buffer.start();};
				LogStream& operator << (const short val) 
				{
					char buff[6];	
					snprintf(buff, 6, "%d", val);
					m_buffer.append(buff);
					return *this;
				};

				LogStream& operator << (const unsigned short val)
				{
					char buff[5];
					snprintf(buff, 5, "%u", val);
					m_buffer.append(buff);
					return *this;
				};

				LogStream& operator << (const int val)
				{
					char buff[11];
					snprintf(buff, 11, "%d", val);
					m_buffer.append(buff);
					return *this;
				};

				LogStream& operator << (const unsigned int val)
				{
					char buff[10];
					snprintf(buff, 10, "%u", val);
					m_buffer.append(buff);
					return *this;
				};

				LogStream& operator << (const long val)
				{
					char buff[20];
					snprintf(buff, 20, "%ld", val);
					m_buffer.append(buff);
					return *this;
				};	

				LogStream& operator << (const unsigned long val)
				{
					char buff[20];
					snprintf(buff, 20, "%lu", val);
					m_buffer.append(buff);
					return *this;
				};

				LogStream& operator << (const long long val)
				{
					char buff[20];
					snprintf(buff, 20, "%lld", val);
					m_buffer.append(buff);
					return *this;
				};

				LogStream& operator << (const unsigned long long val)
				{
					char buff[20];
					snprintf(buff, 20, "%llu", val);
					m_buffer.append(buff);
					return *this;
				};
				
				LogStream& operator << (const char *val)
				{
					m_buffer.append(val);
					return *this;
				};

				LogStream& operator << (const double val)
				{
					char buff[20];
					snprintf(buff, 20, "%.12g", val);
					m_buffer.append(buff);
					return *this;
				};

				template<typename T>
					LogStream& operator << (const T& t)
					{
						m_buffer.append(t.c_str());
						return *this;
					};

			private:	
				LogBuffer	m_buffer;
				static LogLevel g_logLevel;
				static void setLogLevel(LogLevel lvl) {g_logLevel = lvl;};
		};
		LogLevel g_logLevel = INFO;
	}

}

#endif
