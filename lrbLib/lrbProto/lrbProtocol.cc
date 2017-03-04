#include "lrbProtocol.h"
#include "lrbNetWork.h"


using namespace lrb::Protocol;

namespace {

	extern int g_lrb_VerifyProto_protoNum;
	extern void *g_lrb_VerifyProto_ptrs[];
	extern short g_lrb_VerifyProto_confs[][5];

	extern int g_lrb_GameProto_protoNum;
	extern void *g_lrb_GameProto_ptrs[];
	extern short g_lrb_GameProto_confs[][5];

	int s_lrb_proto_nums[] = {
		g_lrb_VerifyProto_protoNum,
		g_lrb_GameProto_protoNum,
	};

	void **s_lrb_proto_unpackDests[] = {
		g_lrb_VerifyProto_ptrs,
		g_lrb_GameProto_ptrs,
	};

	short (*s_lrb_proto_confs[])[5] = {
		g_lrb_VerifyProto_confs,
		g_lrb_GameProto_confs,
	};

}


namespace lrb {

namespace Protocol {


void parseProtoFrame(char *frame, int len, int verify, lrb::NetWork::NetLink *link)
{
	switch(link->currentTType())
	{
	case lrb::NetWork::TerminalType::TT_CLIENT:
	{
		while(len > 0)
		{       
			int plen;                                                                
			memcpy(&plen, frame, sizeof(int));                                       
			frame += sizeof(int);                                                    
			len -= sizeof(int);                                                      

			int uuid = unpackData(frame, plen);                                      
			frame += plen;
			len -= plen;                                                             
		}       

		auto iter = s_ackFuncs.find(g_lrb_protobuf_AckVerifyData.verify);                
		if (iter != s_ackFuncs.end())                                                    
			iter->second();                                                          
	}
	break;

	case lrb::NetWork::TerminalType::TT_SERVER:   
	{
		DataPacker *packer = s_center.getAvailablePacker();                              
		int val = 0;                                                                     
		while(len > 0)                                                
		{
			int plen;                                                                
			memcpy(&plen, frame, sizeof(int));
			frame += sizeof(int);                                                    
			len -= sizeof(int);                                                      

			int uuid = unpackData(frame, plen);                                      
			frame += plen;                                                           
			len -= plen;                                                             

			s_reqFuncs[uuid >> 1](packer); 
			++val;                                                                   
		}       
		packer->setDoneValue(val, verify, link);
	}
	break;

	default:
	break;
	}
}


}

}

