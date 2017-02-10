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
	void bindReqFunc(int uuid, const std::function<void(int, int)> &func);
	void bindAckFunc(int verify, const std::function<void()> &func);
	void parseNetData(char *data, int size);

}

}


#endif
