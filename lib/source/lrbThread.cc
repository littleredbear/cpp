#include "lrbThread.h"


using namespace lrb;

void * lrbThread::ThreadFunc(void *arg)
{
	lrbThread *p = static_cast<lrbThread *>(arg);
	p->_callBack(p->_arg);	
	return 0;	

}

void lrbThread::run() 
{
	pthread_t tidp;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	int n = pthread_create(&tidp, &attr, ThreadFunc, this);	
	if (n != 0) {
		std::string str = "create thread error!";
		throw lrbException(str, 2, n);
	}
		
}
