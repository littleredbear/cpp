#ifndef LRB_EXCEPTION_H
#define LRB_EXCEPTION_H

#include "lrb.h"

namespace lrb {
	class lrbException {
		private:
			int _errNo;			
			char *_msg;
		public:
			lrbException(const char *msg, int errno);
			~lrbException() throw();
			const char *what() const throw() {return this->_msg;};
			int errNo();
	};
/*
	class lrbLogException : public lrb::lrbException {
		public:
			lrbLogException(const char *msg, int errno):lrbException(msg,errno){};
		//errno 1,open error 2.write error };
	};
*/
	class lrbSocketException : public lrb::lrbException {
		//errno 1,open error 2.write error
		public:
			lrbSocketException(const char *msg, int errno):lrbException(msg,errno){};
	};

}


#endif
