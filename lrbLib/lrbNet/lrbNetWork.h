#ifndef _LRB_NET_WORK_H
#define _LRB_NET_WORK_H

#include <string>
#include <vector>
#include <functional>

namespace lrb {

namespace NetWork {

	class NetLink;

	enum class ProtoType {
		PT_LINK,
		PT_GameProto,
		PT_TOP,
	};

//-----------------------------------------Data Packer-------------------------------
        
        class DataPacker {
        public: 
                DataPacker();
                ~DataPacker();
                
		void addValue(int val = 1);
                void packData(void *data, int protoId, ProtoType type);
                void setDoneValue(int val);

		void clearData();
	
		void sendData(int linkId = -1);
		void sendToRoleIds(uint32_t count, ...);

		int getData(void **res);
		void setGroupSend(const std::string &group, short port);
		void roleLogin(uint32_t roleId);

		uint8_t state();
		
		NetLink *netLink();
		void setNetLink(NetLink *link, int verify);

                void bindLastPacker(DataPacker *packer);
                void bindNextPacker(DataPacker *packer);

                DataPacker *lastPacker();
                DataPacker *nextPacker();

        private:
                void reusePacker();

                std::vector<void *> m_datas;
                std::vector<int> m_lens;
		std::string m_group;

                int m_doneVal;
                int m_curVal;
                int m_verify;
                NetLink *m_link;

                DataPacker *m_last;
                DataPacker *m_next;
		
		short m_port;
		uint8_t m_state;
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
		bool checkParseNetFrame(NetLink *link);

                char *m_dataCache;
                int m_cacheLen;
                int m_frameLen;
                int m_verify;
        };

//-------------------------------Net Data----------------------------------

	enum class LinkState {
		LS_CLOSED = 0,
		LS_TOLINK,
		LS_LINKED
	};

	enum class TerminalType {
		TT_NONE,
		TT_CLIENT,
		TT_SERVER,
	};

	class ReuseData {
	public:
		ReuseData();
		~ReuseData();

		void *data();
		void setData(void *data, uint32_t count = 1);
		void retain(uint32_t count = 1);
		void release(uint32_t count = 1);

	private:
		void *m_data;
		uint32_t m_count;
	};

	class NetData {
	public:
		NetData();
		~NetData();

		bool setNetData(int verify, ReuseData *data, size_t size);
		bool writeNetData(int sockfd, int verify, int &off);
		bool empty();

		void bindNextData(NetData *next);
		NetData *nextData();

	private:
		ReuseData *m_data;
		NetData *m_next;

		int m_verify;
		size_t m_size;
	};


//--------------------------------------Net Link---------------------------------

	class NetLink {
	public:
		const static int s_defaultNum = 128;
		static void sendGroupData(const std::string &group, short port, void *data, size_t size);
		static void sendToRoleIds(uint32_t *roleId, uint32_t rcount, void *data, size_t size);

		NetLink();
		~NetLink();

		void addNetData(int verify, void *data, size_t size);
		void addReuseData(int verify, ReuseData *data, size_t size);

		uint32_t roleId();
		void roleLogin(uint32_t roleId);
		void roleLogout();

		void disConnect();
		void connectServer(const std::string &host, const std::string &service);
		void mcastJoinGroup(const std::string &group, const std::string &source, short port, int family); 
		void acceptLink(int sockfd);

		void bindLastLink(NetLink *link);
		void bindNextLink(NetLink *link);

		NetLink *lastLink();
		NetLink *nextLink();

		TerminalType currentTType();
		ProtoType currentProtoType();
		void setProtoType(ProtoType type);

		void processLinkProto(int protoId);
		
	private:
		void mcastJoinIPV4Group(int sockfd, const std::string &group, const std::string &source, short port);
		void sendNetData(int sockfd);
		void readNetData(int sockfd);
		void linkFunc(int sockfd, short events);
		void processReqLinkData();
		void processAckLinkData();
		
		DataParser m_parser;
		std::string m_host;
		std::string m_service;
		std::vector<void *> m_ptrs;

		NetData m_datas[s_defaultNum];

		NetData *m_addData;
		NetData *m_execData;
		NetLink *m_last;
		NetLink *m_next;

		LinkState m_state;
		TerminalType m_ttype;
		ProtoType m_protoType;

		uint32_t m_roleId;
		
		int m_size;
		int m_off;
		int m_verify;
		int m_tcpHandler;
		int m_udpHandler;

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


	void connectServer(const std::string &hostname, const std::string &service, int uuid);
	void mcastJoinGroup(const std::string &group, const std::string &source, short port, int uuid);
	void startService(short service);
	void disConnect(int uuid);
	void sendData(int uuid, void *data, size_t size);
	void sendGroupData(const std::string &group, short port, void *data, size_t size);
    	int packData(const void *data, int uuid, void **res, ProtoType ptype); //res需要自行释放
    	int unpackData(const char *src, int size, ProtoType ptype);
	void bindConnectFunc(const std::function<void(NetLink *)> &func);
	void bindLinkProtoFunc(const std::function<void(NetLink *, int protoId)> &func);
	void bindFinalCheckFunc(const std::function<void()> &func);

}

}


#endif
