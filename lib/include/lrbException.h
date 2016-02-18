#ifndef LRB_EXCEPTION_H
#define LRB_EXCEPTION_H

#include <cstring>
#include <stdlib.h>
#include <string>

enum expType {
	sockExp = 1,
	threadExp	

};

enum sockExp {
	getAddrinfoExp = 1,
	connectExp,
	listenExp1,	// no avaiable address
	listenExp2,	// setsockopt exp
	listenExp3,	// listen error
	dataExp
};

namespace lrb {
	class lrbException {
		private:
			int _errNo;			
			int _type;	
			std::string _msg;
		public:
			lrbException(std::string &msg, int type, int errNo):_msg(msg),_type(type),_errNo(errNo){};
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
