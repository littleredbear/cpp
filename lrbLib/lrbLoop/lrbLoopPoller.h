#ifndef _LRB_LOOP_POLLER_H
#define _LRB_LOOP_POLLER_H


#include "Poller.h"


namespace lrb {

	class LoopPoller {
	public:
		LoopPoller();
		~LoopPoller();

		void poll(int timeout);
		void notify();
		void pollFunc(short events);

	private:
		Poller m_poller;
		int m_ntyFd;
	};

}



#endif
