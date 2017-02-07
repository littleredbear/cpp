#ifndef _LRB_NET_DATA_H
#define _LRB_NET_DATA_H

#include <string>

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

	struct ResVerify {

		int verify;
	};

	int archiveData(const void *src, int uuid, void **res); //res需要自行释放
	void parseData(void *src);

}

}


#endif
