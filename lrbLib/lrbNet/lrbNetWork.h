#ifndef _LRB_NET_WORK_H
#define _LRB_NET_WORK_H

#include "lrbPoller.h"

namespace lrb {

//-------------------------------Net Task----------------------------------
	
	class NetTask {
	public:
		NetTask();
		~NetTask();

		bool setNetTask(int fd, const void *data, size_t size);
		bool execNextTask();

		void bindNextTask(NetTask *next);
		NetTask *nextTask();

	private:
		int m_fd;
		const void *m_data;
		size_t m_size;
		NetTask *m_next;
	};

//---------------------------------------Net Manager------------------------------

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
