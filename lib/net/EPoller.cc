#ifdef LRB_LINUX

#include "EPoller.h"
#include "LogStream.h"
#include <unistd.h>
#include <memory.h>

using namespace lrb::base;

EPoller::EPoller():
	m_epollFd(epoll_create1(EPOLL_CLOEXEC)),
	m_numEvents(0),
	m_events(kInitEventListSize)
{
	assert(m_epollFd > 0);
}

EPoller::~EPoller()
{
	close(m_epollFd);
}

void EPoller::poll(int timeout)
{
	int numEvents = epoll_wait(m_epollFd, &*m_events.begin(),static_cast<int>(m_events.size()),timeout);
	int saveErr = errno;
	if (numEvents > 0) {
		if (numEvents == m_events.size()) {
			m_events.resize(m_events.size()*2);
		}

		for (int i=0;i<numEvents;++i) {
			Channel *channel = static_cast<Channel *>(m_events[i].data.ptr);
			channel->handleEvent(m_events[i].events);
		}

	} else if (numEvents < 0) {
		if (saveErr != EINTR) {
			LOGERR << "epoll wait err! errno:" << saveErr;
		}
	}
}

void EPoller::update(Channel *channel)
{
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = channel->events();
	event.data.ptr = channel;
	int fd = channel->fd();
	int op = operationFromChannel(channel);
	assert(epoll_ctl(m_epollFd, op, fd, &event) == 0);
}

int operationFromChannel(Channel *channel)
{
	switch (channel->state()) {
		case Channel::kNew:
		case Channel::kDeleted:
			return EPOLL_CTL_ADD;

		case Channel::kAdded:	
			return channel->isNoneEvent() ? EPOLL_CTL_DEL : EPOLL_CTL_MOD;
	}

}



#endif
