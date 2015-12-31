#ifndef LRB_LOG_H
#define LRB_LOG_H

#include <../character/character.h>

namespace lrb {

	class lrbLog {
		private:
		lrbLog();
		int _fd;
		public:	
		
		static lrbLog * getInstance();
	};

	void lrbLog(const char *buf, size_t len);
	void lrbError(const char *buf, size_t len);
}



#endif
