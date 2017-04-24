#ifndef _LRB_ERR_CODE_H
#define _LRB_ERR_CODE_H


namespace lrb {

namespace NetWork {
	class DataPacker;
}

namespace server {

	enum class ErrCode {
		Err_None = 0,
		Err_Memory,
		Err_ModId,
		Err_Verify,
		Err_RoleId,
		Err_InFight,
		
	};

	
	void sendErrCode(lrb::NetWork::DataPacker *packer, ErrCode err);

}

}

#endif
