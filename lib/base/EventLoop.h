#ifndef LRB_BASE_EVENTLOOP_H
#define LRB_BASE_EVENTLOOP_H

#include "base/base.h"
#include "base/Mutex.h"
#include "base/Poller.h"
#include "base/EventChannel.h"
#include <memory>
#include <vector>
#include <functional>

namespace lrb {

	namespace base {
//可以由使用者自己控制创建全局loop，在其他线程获得使用
		class EventLoop : public noncopyable {

			friend class Channel;
			public:
				enum EventLevel {
					Normal,
					High
				};

				typedef std::function<void()> Func;

				EventLoop();
				void loop();
				void quit() {m_quit = true;};
				void runInLoop(Func func);
				void runInLoop(Func func, EventLevel lvl);
				
				static EventLoop *currentThreadLoop();
			private:
				void updateChannel(Channel *channel);
				void removeChannel(Channel *channel);
				void createEventChannel();
				bool isCurrentThreadLoop();
				void wakeup();

				bool m_quit;
				int m_awakeFd;
				MutexLock m_lock;
				std::unique_ptr<Poller> m_poller;
				std::unique_ptr<EventChannel> m_eventChannel;
				
		};
	}
}


#endif
