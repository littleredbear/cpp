#ifndef LRB_BASE_EPOLLER_H
#define LRB_BASE_EPOLLER_H

#include "base/base.h"
#include <vector>

namespace lrb {

	namespace base {

		class EPoller : noncopyable {

			public:
				EPoller();
				~EPoller();
				void poll();
				void update(Channel *channel);
				const int numEvents() {return m_numEvents;};
				const std::vector<struct epoll_event> &activeEvents() {return m_events;};

			private:
				
				static const int kInitEventListSize = 16;
	
				int m_epollFd;
				int m_numEvents;
				std::vector<struct epoll_event> m_events;	

		};

	}

}

#endif
