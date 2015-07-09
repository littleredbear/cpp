#ifndef __LRB_MSG_H
#define __LRB_MSG_H


namespace lrb {

		typedef enum _msgType {
				msgType_NONE	=	-1,
				msgType_TCP		=	0,
				msgType_HTTP	=	1
		} msgType;
		
		class msg {
				protected:				
				msgType _type;

				public:
				msg() {_type = msgType_NONE;};
				~msg() {};

				msgType type() {return _type;};
		};
}


#endif
