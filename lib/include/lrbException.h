#ifndef LRB_EXCEPTION_H
#define LRB_EXCEPTION_H

#include <exception>

namespace lrb {

	class lrbLogException : public std::exception {
		private:
		int _errno;	//1,open error 2.write error
		const char *_msg;
		public:
		lrbLogException(const char *msg, int errno);
		virtual const char *what() const throw() {return this->_msg;};
		int errno() {return this->_errno;};
	};


}


#endif
