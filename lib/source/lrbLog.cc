
#include "lrbLog.h"

using namespace lrb;

lrbLog::lrbLog()
{
#ifdef LRBDEBUGLOG
	_fd = STDOUT_FILENO;
#else
	_fd = open(LRB_LOG_PATH, O_WRONLY|O_CREAT|O_APPEND,00644);
	if (_fd == -1) {
		_fd = open("/dev/null",O_RDWR);		
	}
#endif
}

void lrbLog::writeLog(const char *buff, size_t len)
{
	writen(this->_fd, buff, len);
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

void lrb::LrbLOG(const char *buff, size_t len)
{
	lrb::lrbLog *instance = lrbLog::getInstance();
	instance->commitLog(buff, len, 0);	
}

void lrb::LrbERROR(const char *buff, size_t len) 
{
	lrb::lrbLog *instance = lrbLog::getInstance();
	instance->commitLog(buff, len, 1);
}

