#ifndef LRB_CONDITION_H
#define LRB_CONDITION_H

#include <pthread.h>
#include <time.h>

namespace lrb {

	class Condition : public noncopyable {
		public:	
			explicit Condition(MutexLock &mutex): m_mutex(mutex) 
			{
				assert(pthread_cond_init(&m_cond, NULL) == 0);
			};
		
			~Condition(){assert(pthread_cond_destroy(&m_cond) == 0);};
	
			void wait() 
			{
				pthread_cond_wait(&m_cond, m_mutex.getPthreadMutex());
			};

			bool waitForSeconds(int sec)
			{
				struct timespec abstime = {0};
				abstime.tv_sec = time(0) + sec;
				return ETIMEDOUT == pthread_cond_timedwait(&m_cond, m_mutex.getPthreadMutex(), &abstime);
			};

			void notify()
			{
				assert(pthread_cond_signal(&m_cond) == 0);
			};

			void notifyAll()
			{
				assert(pthread_cond_broadcast(&m_cond) == 0);
			};

		private:
			MutexLock &m_mutex;
			pthread_cond_t m_cond;
	};
}


#endif
