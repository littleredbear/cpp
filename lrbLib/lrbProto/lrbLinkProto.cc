#include "lrbLinkProto.h"


using namespace lrb::LinkProto;

ReqStreamData g_lrb_LinkProto_ReqStreamData;
ReqLinkData g_lrb_LinkProto_ReqLinkData;
AckStreamData g_lrb_LinkProto_AckStreamData;
AckLinkData g_lrb_LinkProto_AckLinkData;

void *g_lrb_LinkProto_ptrs[] = {
&g_lrb_LinkProto_ReqStreamData,
&g_lrb_LinkProto_AckStreamData,
&g_lrb_LinkProto_ReqLinkData,
&g_lrb_LinkProto_AckLinkData,
};

short g_lrb_LinkProto_confs[][5] = {
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
{0*sizeof(std::string),0,4,0,0},
};

namespace lrb {

namespace LinkProto {

void *getUnpackDest(int protoId)
{
	if (protoId < 0 || protoId >= 4)
		return NULL;

	return g_lrb_LinkProto_ptrs[protoId];
}

short *getProtoConfs(int protoId)
{
	if (protoId < 0 || protoId >= 4)
		return NULL;

	return g_lrb_LinkProto_confs[protoId];
}


}
}

