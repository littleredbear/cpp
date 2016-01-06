#ifndef LRB_LOG_H
#define LRB_LOG_H

#include "../lrb.h"

#define	LRB_LOG_PATH	/Users/reid/Develop/cpp/log/log.txt	

namespace lrb {

	class lrbLog {
		private:
		lrbLog() throw(lrgLogException);
		int _fd;
		public:	
		void writeLog(const char *buff, size_t len);
		void commitLog(const char *buff, size_t len, char type);		
		static lrbLog *getInstance();
	};

}



#endif
