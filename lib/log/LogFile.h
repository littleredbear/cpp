#ifndef LRB_LOG_LOGFILE_H
#define LRB_LOG_LOGFILE_H

#include <string>

namespace lrb {

	namespace log {

		const int krollSize = 1024*1024;
		const int krollTime = 60*60*24;

		class LogFile {
			public:
				explicit LogFile(const char *basename);
				~LogFile();
				void append(const char *buff, int len);
				void flush();

			private:
				void rollFile();
				std::string LogFileName();
				FILE *m_fp;
				int m_logSize;
				time_t m_logTime;	
				std::string m_basename;
		};

	}
}

#endif
