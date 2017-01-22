#ifndef _LRB_NET_WORK_H
#define _LRB_NET_WORK_H

#include "lrbPoller.h"

namespace lrb {
	
	class NetTask {
	public:
		NetTask();
		~NetTask();

		bool setNetTask(void *data, size_t size);

		void bindNextTask(NetTask *next);
		NetTask *nextTask();

	private:
		void *m_data;
		size_t m_size;
		NetTask *m_next;
	};

	class NetManager {

	};

	class NetWork {
	public:
		static void connect();
		static void disConnect(int fd);
		static void sendData(int fd, const char *data, size_t size);
		static void eventFunc(int fd, short events);

	private:
		Poller m_poller;

	};

}


#endif
