#ifndef LRB_EXCEPTION_H
#define LRB_EXCEPTION_H

#include <cstring>
#include <stdlib.h>
#include <string>

enum expType {
	sockExp = 1

};

enum sockExp {
	connectExp = 1,
	listenExp
};

namespace lrb {
	class lrbException {
		private:
			char _errNo;			
			char _type;	
			std::string _msg;
		public:
			lrbException(std::string &msg, char type, char errNo):_msg(msg),_type(type),_errNo(errNo){};
			~lrbException() throw(){};
			const char *what() const throw() {return this->_msg.c_str();};
			int errNo() {return this->_errNo;};
	};
/*
	class lrbLogException : public lrb::lrbException {
		public:
			lrbLogException(const char *msg, int errno):lrbException(msg,errno){};
		//errno 1,open error 2.write error };
	};
*/
/*
	class lrbSocketException : public lrb::lrbException {
		//errno 1,open error 2.write error
		public:
			lrbSocketException(const char *msg, int errNo):lrbException(msg,errNo){};
	};
*/

}


#endif
