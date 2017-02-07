#ifndef _LRB_POLLER_H
#define _LRB_POLLER_H

#include <poll.h>
#include <vector>
#include <functional>

namespace lrb {

	class Poller {
	public:
		Poller();
		~Poller();

		void poll(int timeout);
		int addPollFd(int fd, short events, const std::function<void(int, short)> &func);
		void removePollFd(int handler);
		void updatePollFd(int handler, short events, const std::function<void(int, short)> &func);

	private:
		std::vector<pollfd> m_pfds;
		std::vector<int> m_handlers;
		std::vector<int> m_rhandlers;
		std::vector<int> m_fhandlers;
		std::vector<std::function<void(int, short)> > m_funcs;

	};

}



#endif
