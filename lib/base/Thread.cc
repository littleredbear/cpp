
#include "Thread.h"
#include "Exception.h"

namespace lrb {

namespace currentThread {
	__thread int t_cachedTid = 0;
}

namespace detail {
	struct ThreadData {
		typedef base::Thread::ThreadFunc ThreadFunc;
		ThreadFunc _func;

		ThreadData(ThreadFunc func):_func(func) {};
		void run()
		{
			try {
				_func();
			} catch (...) {
				abort();
			}
		};	

	};

	void * startThread(void *arg)
	{
		ThreadData *data = static_cast<ThreadData *>(arg);
		data->run();	
		delete data;
		return 0;	

	}
}
}

using namespace lrb;
using namespace lrb::base;
Thread::Thread(ThreadFunc func):
	_func(func),
	_started(false),
	_tid(0)
{

}

Thread::~Thread()
{
	if (_started && !_joined) {
		pthread_detach(_tid);
	}	
}

void Thread::run() 
{
	_started = true;
	detail::ThreadData *data = new detail::ThreadData(_func);
	if (pthread_create(&_tid, NULL, detail::startThread, data)){
		delete data;
		std::string s = "create thread failed!";
		throw Exception(s, 2, 1);
	}	
}

int Thread::join()
{
	_joined = true;		
	return pthread_join(_tid, NULL);
}


