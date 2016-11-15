#ifndef LRB_BASE_THREAD_H
#define LRB_BASE_THREAD_H

#include <pthread.h>
#include <functional>
#include "base.h"

namespace lrb {

	namespace base {

		class Thread : public noncopyable {
			public:
				typedef std::function<void()> ThreadFunc;
			private:
				ThreadFunc _func;
				bool	_started;
				bool	_joined;
				pthread_t _tid;

			public:
				explicit Thread(ThreadFunc func);
				~Thread();	
				void run();		
				int join();
		};

	}
}

#endif
