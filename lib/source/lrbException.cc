#include "lrbException.h"

using namespace lrb;

lrbException::lrbException(const char *msg, int errno)
{
	unsigned int len = strlen(msg);
	char *p = (char *)calloc(len+1,sizeof(char));
	if (p == NULL) 
		return;

	memcpy(p,msg,len+1);	
	this->_msg = p;
	this->_errNo = errno;
}

lrbException::~lrbException() throw()
{
	free(this->_msg);
}

int lrbException::errNo()
{
	return this->_errNo;
}
