#ifndef LRB_LOG_H
#define LRB_LOG_H

#include <time.h>
#include <cstring>
#include <string>

#define	LRB_LOG_PATH	("/Users/reid/Develop/cpp/log/log.txt")	

namespace lrb {

	void LrbLOG(const char *buff, size_t len);
	void LrbERROR(const char *buff, size_t len);

	class lrbLog {
		private:
		lrbLog();
		int _fd;
		public:	
		void writeLog(const char *buff, size_t len);
		void commitLog(const char *buff, size_t len, char type);		
		static lrbLog *getInstance();
	};

}



#endif
