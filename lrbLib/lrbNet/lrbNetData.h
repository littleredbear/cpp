#ifndef _LRB_NET_DATA_H
#define _LRB_NET_DATA_H

#include <string>
#include <functional>


#define LRB_APPSERVER

namespace lrb {

namespace NetData {

	struct ReqVerifyData {
//string

//8

//4
		int verify;
//2

//1

	};

	struct AckVerifyData {

		int verify;
	};

//-----------------------------------------Data Packer-------------------------------

	class DataPacker {
	public:
		DataPacker();
		~DataPacker();

		void packData(void *data, int size, bool verify = false);
		void setDoneValue(int val, int uuid, int verify);

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
		int m_uuid;
		int m_verify;

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
	
		void parseNetData(char *data, int size);	

	private:
		void parseFirstData(char *data, int size);
		void parseNetFrame(char *frame, int len);
			
		char *m_dataCache;
		int m_cacheLen;
		int m_frameLen;

	};

	int packData(const char *src, int uuid, void **res); //res需要自行释放
	int unpackData(const char *src, int size);
	void bindReqFunc(int uuid, const std::function<void(DataPacker *)> &func);
	void bindAckFunc(int verify, const std::function<void()> &func);

}

}


#endif
