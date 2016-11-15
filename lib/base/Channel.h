#ifndef LRB_BASE_CHANNEL_H
#define LRB_BASE_CHANNEL_H


#include "base/base.h"
#include "base/EventLoop.h"
#include <poll.h>
#include <functional>


namespace lrb {

	namespace base {

		class Channel : public noncopyable {
		
			friend class Poller;	
			public:
				typedef std::function<void()> EventCallback;

				Channel(const EventLoop *loop, int fd):m_loop(loop), m_fd(fd), m_events(0), m_index(-1) {};
				int fd() {return m_fd;};
				void enableReading();
				void disableReading();
				void enableWriting();
				void disableWriting();
				void close();

			private:
				void handleEvents();
				short events() {return m_events;};
				void setrevents(short revents) {m_revents = revents;};
				void setIndex(int idx) {m_index = idx;};
				int index() {return m_index;};

				EventLoop *m_loop;
				int m_fd;
				short m_events;
				short m_revents;
				int m_index;
				EventCallback m_readCallback;
				EventCallback m_writeCallback;
				EventCallback m_closeCallback;
		};
	}
}


#endif
