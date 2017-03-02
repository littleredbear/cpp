#ifndef _LRB_PROTO_BUF_H
#define _LRB_PROTO_BUF_H

#include <string>

//整形必须用int32_t
//结构排序string,8,4,2,1字节

namespace lrb {

namespace ProtoBuf {

	struct ReqVerifyData {
		uint32_t verify;
	};
		
	struct AckVerifyData {
		uint32_t verify;
	};

	struct ReqRoleInfo {
		std::string name;
		uint32_t roleId;
	};

	struct AckRoleInfo {
		std::string name;
		uint32_t roleId;
	};

}

}


#endif
