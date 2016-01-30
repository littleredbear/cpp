#ifndef _LRB_SOCKET_H
#define _LRB_SOCKET_H

#include "lrb.h"
#include <sys/socket.h>

namespace lrb {

	class lrbSocket {

		private:	
			int _fd;
			char _buff[1024];
			~lrbSocket();	

		public:
			lrbSocket() throw(lrbSocketException);

	};

}


#endif
