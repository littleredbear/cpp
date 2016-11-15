#ifndef LRB_LOG_LOGBUFFER_H
#define LRB_LOG_LOGBUFFER_H

#include <iostream>
#include "base.h"
#include "Buffer.h"
#include "Thread.h"
#include "LogFile.h"
#include "Mutex.h"
#include <list>
#include <stdlib.h>
#include <iostream>
#include "Condition.h"

namespace lrb {

	namespace log {

		const int kSmallBuffer = 4096;
		const int kFlushSeconds = 3;

		class LogBuffer : public base::noncopyable {
			public:
				typedef base::Buffer<kSmallBuffer> SBuffer;
				LogBuffer(const char *basename);
				void append(const char *buff, unsigned int len);	
				void start() {m_running = true;m_thread.run();};
				void flush();

			private:
				bool m_running;
				LogFile m_file;
				base::MutexLock m_mutex;
				base::Condition m_cond;
				base::Thread m_thread;
				std::shared_ptr<SBuffer> m_buffer;
				std::list<std::shared_ptr<SBuffer> > m_list;

				void ThreadFunc();
		};
	}
}


#endif
