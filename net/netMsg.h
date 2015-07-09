#ifndef __LRB_MSGNET_H
#define __LRB_MSGNET_H

#include "../base/msg.h"

namespace lrb {
			
		class tcpMsg : public msg {
			
				private:
				const char *_tcpName;	
				char *_data;
				int _dataLength;
		
				private:
				tcpMsg(const char *name) : _tcpName(name) {_type = msgType_TCP;};
				~tcpMsg() {};
			
				public:
				const char *name() {return _tcpName;};
				void setMsgData(char *data, int len) {_data = data;_dataLength = len;};
				char *msgData() {return _data;};
				int dataLen() {return _dataLength;};
		};
		
		class httpMsg : public msg {
		
		};
}


#endif
