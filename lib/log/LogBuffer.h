#ifndef LRB_LOGBUFFER_H
#define LRB_LOGBUFFER_H

#include <iostream>
#include "base/lrb.h"
#include "base/Buffer.h"
#include "base/Thread.h"
#include "log/LogFile.h"
#include "base/Mutex.h"
#include <list>
#include <stdlib.h>
#include <iostream>
#include "base/Condition.h"

namespace lrb {

	namespace log {
		const int kSmallBuffer = 5;
		const int kFlushSeconds = 3;

		class LogBuffer : public noncopyable {
			public:
				typedef Buffer<kSmallBuffer> SBuffer;
				LogBuffer();
				void append(const char *buff);	
				void start() {m_running = true;m_thread.run();};
				void flush();

			private:
				bool m_running;
				LogFile m_file;
				MutexLock m_mutex;
				Condition m_cond;
				Thread m_thread;
				std::shared_ptr<SBuffer> m_buffer;
				std::list<std::shared_ptr<SBuffer> > m_list;

				void ThreadFunc();
		};
	}
}


#endif
