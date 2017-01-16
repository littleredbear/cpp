#include "lrbLoopPoller.h"
#include <sys/socket.h>


using namespace lrb;


LoopPoller::LoopPoller()
{
	int fds[2];
	assert(sodketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0);
	m_poller.addPollFd(fds[0], POLLIN, std::bind(&LoopPoller::pollFunc, this));
	m_ntyFd = fds[1];
}

LoopPoller::~LoopPoller()
{

}

void LoopPoller::poll(int timeout)
{
	m_poller.poll(timeout);
}

void LoopPoller::notify()
{

}

void LoopPoller::pollFunc(short events)
{

}

