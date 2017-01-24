#ifndef _LRB_NET_WORK_H
#define _LRB_NET_WORK_H

#include "lrbPoller.h"

namespace lrb {

namespace NetWork {

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

//---------------------------------------Data Manager------------------------------

	class DataManager {
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

//-----------------------------------Link Data----------------------------------

	class LinkData {
	public:
		LinkData();
		~LinkData();

		void setNetLink(NetLink *link);
		NetLink *getNetLink();
		
		void nextVerify();
		int getVerify();
	
	private:
		NetLink *m_link;
		int m_verify;
	};

//--------------------------------Link Manager------------------------------------

	class LinkManager {
	public:
		const static int s_maxLinkNum = (1 << 16);

		NetLink *getNetLink(int uuid, int verify);

	private:
		
	};


	void connectServer(const char *addr, short port);
	void startServer(const char *addr, short port);
	void disConnect(int uuid);
	void sendData(int uuid, int verify, const char *data, size_t size);

}

}


#endif
