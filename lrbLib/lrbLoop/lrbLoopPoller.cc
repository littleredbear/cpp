#include "lrbLoopPoller.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <functional>


using namespace lrb;


LoopPoller::LoopPoller()
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

	m_poller.addPollFd(fds[1], POLLIN, std::bind(&LoopPoller::pollFunc, this, std::placeholders::_1, std::placeholders::_2));
	m_ntyFd = fds[0];
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
	char one = 1;
	write(m_ntyFd, &one, sizeof(one));
}

void LoopPoller::pollFunc(int fd, short events)
{
	char one;
	read(fd, &one, sizeof(one));
}

void LoopPoller::addPollFd(int fd, short events, const std::function<void(int, short)> &func)
{
	m_poller.addPollFd(fd, events, func);
}

