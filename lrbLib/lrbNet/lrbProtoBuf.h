#ifndef _LRB_PROTO_BUF_H
#define _LRB_PROTO_BUF_H

#include <string>

namespace lrb {

namespace ProtoBuf {

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

	struct ReqRoleInfo {
		std::string name;
		int roleId;

	};

	struct AckRoleInfo {
		std::string name;
		int roleId;
	};

}

}


#endif
