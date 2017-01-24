#ifndef _LRB_NET_WORK_H
#define _LRB_NET_WORK_H

#include "lrbPoller.h"

namespace lrb {

//-------------------------------Net Data----------------------------------
	
	class NetData {
	public:
		NetData();
		~NetData();

		bool setNetData(int uuid, int verify, void *data, size_t size);
		bool writeNetData(int &off);
		bool empty();

		void bindNextData(NetData *next);
		NetData *nextData();

	private:
		int m_uuid;
		int m_verify;
		void *m_data;
		size_t m_size;
		NetData *m_next;
	};

//---------------------------------------Net Manager------------------------------

	class NetManager {
	public:
		const static int s_defaultNum = 128;
	
		NetManager();
		~NetManager();
		void addNetData(int uuid, void *data, size_t size);
		
	private:
		NetData m_datas[s_defaultNum];
		NetData *m_addData;
		NetData *m_getData;
	};

//-------------------------------------Net Task-------------------------------

	class NetTask {

	private:
		NetData *m_data;
		NetTask *m_next;
	};

//--------------------------------------Net Link---------------------------------

	class NetLink {
	public:
		void disconnect();
		
	private:
		NetTask *m_task;
		int m_off;
		int m_fd;
		int m_verify;
		int m_handler;

	};

	class LinkManager {
	public:
		NetLink *getNetLink(int uuid, int verify);

	private:
		

	};


	class NetWork {
	public:
		static void connectServer(const char *addr, short port);
		static void startServer(const char *addr, short port);
		static void disConnect(int uuid);
		static void sendData(int uuid, const char *data, size_t size);

	private:

	};

}


#endif
