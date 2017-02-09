#ifndef _LRB_NET_WORK_H
#define _LRB_NET_WORK_H

#include <string>
#include <vector>

namespace lrb {

namespace NetWork {

	enum LinkState {
		LS_CLOSED = 0,
		LS_TOLINK,
		LS_LINKED
	};

//-------------------------------Net Data----------------------------------
	
	class NetData {
	public:
		NetData();
		~NetData();

		bool setNetData(int verify, void *data, size_t size);
		bool writeNetData(int sockfd, int verify, int &off);
		bool empty();

		void bindNextData(NetData *next);
		NetData *nextData();

	private:
		int m_verify;
		void *m_data;
		size_t m_size;
		NetData *m_next;
	};


//--------------------------------------Net Link---------------------------------

	class NetLink {
	public:
		const static int s_defaultNum = 128;

		NetLink();
		~NetLink();

		void addNetData(int verify, void *data, size_t size);

		void disConnect();
		void connectServer(const std::string &host, const std::string &service);
		void acceptLink(int sockfd, int handler);
		
	private:
		void sendNetData();
		void readNetData();
		void linkFunc(int sockfd, short events);
		
		std::string m_host;
		std::string m_service;
		std::vector<void *> m_ptrs;

		NetData m_datas[s_defaultNum];
		NetData *m_addData;
		NetData *m_execData;
		int m_size;

		LinkState m_state;
		int m_off;
		int m_fd;
		int m_verify;
		int m_handler;

		short m_events;

	};

//---------------------------------Link Acceptor-------------------------------

	class LinkAcceptor {
	public:
		void startService(short service);

	private:
		void acceptFunc(int fd, short events);
		void verifyFunc(int sockfd, short events, int *handler);
	};

//--------------------------------Link Manager------------------------------------

	void connectServer(const std::string &hostname, const std::string &service, int uuid);
	void startService(short service);
	void disConnect(int uuid);
	void sendData(int uuid, int verify, void *data, size_t size);

}

}


#endif
