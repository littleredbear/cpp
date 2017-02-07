#ifndef _LRB_NET_WORK_H
#define _LRB_NET_WORK_H

#include <string>

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
	
		DataManager();
		~DataManager();
		void addNetData(int uuid, void *data, size_t size);
		
	private:
		NetData m_datas[s_defaultNum];
		NetData *m_addData;
		NetData *m_getData;
	};

//-------------------------------------Net Task-------------------------------

	class NetTask {
	public:
		NetTask();
		~NetTask();
		
		void setNetData(NetData *data);
		NetData *getNetData();
		void taskDone();
		void bindNextTask(NetTask *next);
		NetTask *nextTask();

	private:
		NetData *m_data;
		NetTask *m_next;
	};

//--------------------------------------Net Link---------------------------------

	class NetLink {
	public:
		const static int s_defaultTask = 64;

		NetLink();
		~NetLink();

		void nextVerify();
		int getVerify();
		void disConnect();
		void connectServer(const std::string &host, const std::string &service);
		void startService(const std::string &service);
		
	private:
		NetTask m_tasks[s_defaultTask];
		NetTask *m_addTask;
		NetTask *m_execTask;

		int m_off;
		int m_fd;
		int m_verify;
		int m_handler;
		std::string m_host;
		std::string m_service;
	};

//--------------------------------Link Manager------------------------------------


	void connectServer(const std::string &hostname, const std::string &service, int uuid);
	void startService(const std::string &service);
	void disConnect(int uuid);
	void sendData(int uuid, int verify, void *data, size_t size);
	void sortNetData();

}

}


#endif
