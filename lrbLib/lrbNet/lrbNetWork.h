#ifndef _LRB_NET_WORK_H
#define _LRB_NET_WORK_H

#include <string>
#include <vector>
#include <functional>

namespace lrb {

namespace NetWork {

	class NetLink;

//-----------------------------------------Data Packer-------------------------------
        
        class DataPacker {
        public: 
                DataPacker();
                ~DataPacker();
                
                void packData(void *data, int size, bool verify = false);
                void setDoneValue(int val, int verify, NetLink *link);

                void bindLastPacker(DataPacker *packer);
                void bindNextPacker(DataPacker *packer);

                DataPacker *lastPacker();
                DataPacker *nextPacker();

        private:
                void sendData();
                void reusePacker();

                std::vector<void *> m_datas;
                std::vector<int> m_lens;

                int m_doneVal;
                int m_curVal;
                int m_verify;
                NetLink *m_link;

                DataPacker *m_last;
                DataPacker *m_next;

        };

//-------------------------------------------Data Center------------------------------

        class DataCenter {
        public:
                const static int s_defaultNum = 128;

                DataCenter();
                ~DataCenter();

                DataPacker *getAvailablePacker();
                void reusePacker(DataPacker *packer);

        private:
                DataPacker m_packers[s_defaultNum];
                int m_size;
                DataPacker *m_head;
                DataPacker *m_able;
                DataPacker *m_back;

        };

//------------------------------------------Data Parser--------------------------------

        class DataParser {
        public:
                DataParser();
                ~DataParser();

                void parseNetData(char *data, int size, int verify, NetLink *link);

        private:
                void parseFirstData(char *data, int size, int verify, NetLink *link);
                void parseNetFrame(char *frame, int len, int verify, NetLink *link);

                char *m_dataCache;
                int m_cacheLen;
                int m_frameLen;
                int m_verify;
        };

        int packData(const char *src, int uuid, void **res); //res需要自行释放
        int unpackData(const char *src, int size);
        void bindReqFunc(int uuid, const std::function<void(DataPacker *)> &func);
        void bindAckFunc(int verify, const std::function<void()> &func);

//-------------------------------Net Data----------------------------------

	enum LinkState {
		LS_CLOSED = 0,
		LS_TOLINK,
		LS_LINKED
	};

	enum TerminalType {
		TT_NONE,
		TT_CLIENT,
		TT_SERVER,
	};

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
		void acceptLink(int sockfd);

		void bindLastLink(NetLink *link);
		void bindNextLink(NetLink *link);

		NetLink *lastLink();
		NetLink *nextLink();

		TerminalType currentTType();
		int currentProtoId();
		
	private:
		void sendNetData();
		void readNetData();
		void linkFunc(int sockfd, short events);
		
		DataParser m_parser;
		std::string m_host;
		std::string m_service;
		std::vector<void *> m_ptrs;

		NetData m_datas[s_defaultNum];
		NetData *m_addData;
		NetData *m_execData;
		int m_size;

		LinkState m_state;
		TerminalType m_ttype;
		int m_protoId;
		int m_off;
		int m_fd;
		int m_verify;
		int m_handler;

		NetLink *m_last;
		NetLink *m_next;

		short m_events;

	};

//--------------------------------Link Manager------------------------------------

	class LinkManager {
	public:
		const static int s_defaultNum = 128;
		LinkManager();
		~LinkManager();

		NetLink *getAvailableLink();
		void reuseNetLink(NetLink *link);
		
	private:
		NetLink m_links[s_defaultNum];
		int m_size;
		NetLink *m_head;
		NetLink *m_able;
		NetLink *m_back;

	};

//---------------------------------Link Acceptor-------------------------------

	class LinkAcceptor {
	public:
		void startService(short service);

	private:
		void acceptFunc(int fd, short events);
	};


	void connectServer(const std::string &hostname, const std::string &service, int uuid, int protoId);
	void startService(short service);
	void disConnect(int uuid);
	void sendData(int uuid, int verify, void *data, size_t size);
        int packData(const char *src, int uuid, void **res); //res需要自行释放
        int unpackData(const char *src, int size);
        void bindReqFunc(int uuid, const std::function<void(DataPacker *)> &func);
        void bindAckFunc(int verify, const std::function<void()> &func);
	void bindConnectFunc(const std::function<void(NetLink *)> &func);

}

}


#endif
