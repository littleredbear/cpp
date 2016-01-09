
#include "lrbLog.h"

using namespace lrb;

lrbLog::lrbLog() throw(lrbLogException)
{
#ifdef LRBDEBUGLOG
	_fd = STDOUT_FILENO;
#else
	_fd = open(LRB_LOG_PATH, O_WRONLY|O_CREAT|O_APPEND,00644);
	if (_fd == -1)
		throw lrbLogException("open log file fail!", 1);	
#endif
}

void lrbLog::writeLog(const char *buff, size_t len)
{
	int n = writen(this->_fd, buff, len);
	if (n == -1)
		throw lrbLogException("write log file error!", 2);

}

void lrbLog::commitLog(const char *buff, size_t len, char type)
{
	struct tm *tb;
	time_t t = time(0);
        tb = localtime(&t);
        char *ts = asctime(tb);
	this->writeLog(ts, strlen(ts)-1);
	if (type == 0) {
		char l[] = " LOG: ";
		this->writeLog(l, strlen(l));
	} else {
		char l[] = " ERROR: ";
		this->writeLog(l, strlen(l));
	}
	this->writeLog(buff, len);	
	char r[]="\n";
	this->writeLog(r, strlen(r));
}

lrbLog *lrbLog::getInstance()
{
	static lrb::lrbLog lrblog;
	return &lrblog;
}

void LrbLOG(const char *buff, size_t len) throw(lrbLogException) 
{
	lrb::lrbLog *instance = lrbLog::getInstance();
	instance->commitLog(buff, len, 0);	
}

void LrbERROR(const char *buff, size_t len) throw(lrbLogException)
{
	lrb::lrbLog *instance = lrbLog::getInstance();
	instance->commitLog(buff, len, 1);
}

