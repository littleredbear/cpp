#ifndef _LRB_LOOP_POLLER_H
#define _LRB_LOOP_POLLER_H


#include "lrbPoller.h"


namespace lrb {

	class LoopPoller {
	public:
		LoopPoller();
		~LoopPoller();

		void poll(int timeout = -1);
		void notify();
		void pollFunc(int fd, short events);
		void addPollFd(int fd, short events, const std::function<void(int, short)> &func);

	private:
		int m_ntyFd;
		Poller m_poller;
	};

}



#endif
