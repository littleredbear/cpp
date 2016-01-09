#include "lrbException.h"

using namespace lrb;

lrbLogException::lrbLogException(const char *msg, int errno)
{
	this->_msg = msg;
	this->_errno = errno;
}
