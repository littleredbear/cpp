#ifndef _LRB_NET_DATA_H
#define _LRB_NET_DATA_H

#include <string>
#include <functional>


//LRB_APPTYPE 0.兼容,1.服务器,其他.客户端

namespace lrb {

namespace NetData {

	struct ReqVerifyData {
//string

//8

//4
		int uuid;
		int verify;
//2

//1

	};

	struct AckVerifyData {

		int uuid;
		int verify;
	};

	int packData(const void *src, void **res); //res需要自行释放
	void unpackData(void *src);
	void bindReqFunc(int uuid, const std::function<void()> &func);
	void bindAckFunc(int uuid, const std::function<void()> &func);

}

}


#endif
