#include "base/EventLoop.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

namespace {

	__thread EventLoop *t_loopInThisThread = 0;

}

using namespace lrb::base;

EventLoop *currentThreadLoop()
{
	return t_loopInThisThread;
}

EventLoop::EventLoop():
	m_quit(false),
	m_poller(new Poller())
{
	assert(t_loopInThisThread == 0);
	t_loopInThisThread = this;
	createEventChannel();
}

void EventLoop::loop()
{
	while(!m_quit && !m_poller->empty()) {
		m_poller->poll();
	}
}

void EventLoop::runInLoop(Func func)
{
	m_eventChannel->addFunc(func, Normal);
}

void EventLoop::runInLoop(Func func, EventLevel lvl)
{
	m_eventChannel->addFunc(func, lvl);
}

void EventLoop::updateChannel(Channel *channel)
{
	m_poller->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
	assert(isCurrentThreadLoop());
	m_poller->removeChannel(channel);
}

void EventLoop::createEventChannel() 
{
	int fds[2];
	assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0);
	
	int flags = fcntl(fds[0], F_GETFL);
	assert(flags != -1);
	assert(fcntl(fds[0], F_SETFL, flags | O_NONBLOCK) != -1);
	flags = fcntl(fds[1], F_GETFL);
	assert(flags != -1);
	assert(fcntl(fds[1], F_SETFL, flags | O_NONBLOCK) != -1);

	int len = 1;
	assert(setsockopt(fds[0], SOL_SOCKET, SO_SNDBUF, &len, sizeof(len)) == 0);
	assert(setsockopt(fds[1], SOL_SOCKET, SO_RCVBUF, &len, sizeof(len)) == 0);
	m_awakeFd = fds[0];

	m_eventChannel.reset(new EventChannel(this, fds[1]));
	m_eventChannel->enableReading();
}

bool EventLoop::isCurrentThreadLoop() 
{
	return EventLoop::currentThreadLoop() == this;
}

void EventLoop::wakeup() 
{
	char one = 1;
	write(m_awakeFd, &one, sizeof(one));	
}

