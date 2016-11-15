#ifndef LRB_BASE_EVENTCHANNEL_H
#define LRB_BASE_EVENTCHANNEL_H


#include "Channel.h"
#include "Mutex.h"
#include <functional>
#include <vector>


namespace lrb {

	namespace base {

		class EventChannel : public Channel {

			public:
				typedef std::function<void()> Func;
				
				EventChannel(EventLoop *loop, int fd) : Channel(loop, fd) {};
				void addFunc(Func func, EventLoop::EventLevel lvl);

			private:
				
				
				MutexLock m_lock;
				std::vector<Func> m_funcsNormal;
				std::vector<Func> m_funcsHigh;
		};
	}
}


#endif
