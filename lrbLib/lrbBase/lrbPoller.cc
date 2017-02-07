#include "lrbPoller.h"
#include <assert.h>
#include <errno.h>
#include <algorithm>


using namespace lrb;

Poller::Poller()
{

}

Poller::~Poller()
{

}

void Poller::poll(int timeout)
{
	if (::poll(&*m_pfds.begin(), m_pfds.size(), timeout) < 0)
	{
		if (errno != EINTR) 
		{
			// to be continued
			return;
		}
	}
	
	for (int i = m_pfds.size() - 1;i >= 0; --i)
	{
		if (m_pfds[i].revents == 0)
			continue;
		
		if (m_funcs[i])
			m_funcs[i](m_pfds[i].fd, m_pfds[i].revents);
	}
}

int Poller::addPollFd(int fd, short events, const std::function<void(int, short)> &func)
{
	pollfd pfd;
	pfd.fd = fd;
	pfd.events = events;
	pfd.revents = 0;
	m_pfds.push_back(pfd);
	m_funcs.push_back(func);

	int idx = 0;
	if (m_rhandlers.empty()) 
	{
		m_handlers.push_back(m_pfds.size() - 1);
		idx = m_handlers.size() - 1;
	} else
	{
		idx = m_rhandlers.back();
		m_rhandlers.pop_back();
		m_handlers[idx] = m_pfds.size() - 1;
	}

	m_fhandlers.push_back(idx);
	
	return idx;
}

void Poller::removePollFd(int handler)
{
	assert(handler <= m_handlers.size());

	int idx = m_handlers[handler];
	if (idx >= m_pfds.size())
	{
		return;
	}
	else if (idx < m_pfds.size() - 1)
	{
		int hidx = m_fhandlers.back();
		m_handlers[hidx] = idx;
		m_fhandlers[idx] = hidx;

		iter_swap(m_pfds.begin() + idx, m_pfds.end() - 1);
		iter_swap(m_funcs.begin() + idx, m_funcs.end() - 1);
	} 
		
	m_pfds.pop_back();
	m_funcs.pop_back();
	m_fhandlers.pop_back();
	m_rhandlers.push_back(handler);
}

void Poller::updatePollFd(int handler, short events, const std::function<void(int, short)> &func)
{
	assert(handler <= m_handlers.size());
	
	int idx = m_handlers[handler];
	if (idx >= m_pfds.size())
		return;
	
	m_pfds[idx].events = events;
	m_pfds[idx].revents = 0;
	
	if (func)
		m_funcs[idx] = func;
}


