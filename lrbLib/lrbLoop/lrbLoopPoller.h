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
		int addPollFd(int fd, short events, const std::function<void(int, short)> &func);
		void updatePollFd(int handler, short events, const std::function<void(int, short)> &func);
		void removePollFd(int handler);

	private:
		Poller m_poller;
		int m_ntyFd;
	};

}



#endif
