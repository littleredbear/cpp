#ifndef LRB_THREAD_H
#define LRB_THREAD_H

#include <pthread.h>
#include <functional>

namespace lrb {
	class lrbThread {
		typedef std::function<void(void *)> ThreadCallBack;
		private:
			void *_arg;
			static void *ThreadFunc(void *arg);
			ThreadCallBack _callBack;
			
		public:
			lrbThread(ThreadCallBack cb, void *arg = NULL):_callBack(cb),_arg(arg) {};
			~lrbThread(){};	
			void run();		

	};

}

#endif
