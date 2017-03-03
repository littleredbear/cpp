#include "lrbProtocol.h"
#include "lrbNetWork.h"


using namespace lrb::Protocol;

namespace {

	int (*s_proto_packer[])(const char *, int, void **) = {

	};

	int (*s_proto_unpacker[])(const char *, int) = {

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

