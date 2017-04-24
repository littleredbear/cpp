#include "lrbErrCode.h"
#include "lrbNetWork.h"
#include "lrbGameProtoFunc.h"


namespace lrb {

namespace server {

void sendErrCode(lrb::NetWork::DataPacker *packer, ErrCode err)
{
	packer->clearData();
	lrb::GameProto::packAckErrCode(packer, (uint32_t)err);
	packer->sendData();
}

}

}
