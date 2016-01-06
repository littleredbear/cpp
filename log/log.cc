
#include "log.h"

using namespace lrb;

lrbLog::lrbLog()
{
#ifdef LRBDEBUGLOG
	_fd = STDOUT_FILENO;
#else
	_fd = open(LRB_LOG_PATH, O_WRONLY|OCREAT|O_APPEND,00644);
	if (_fd = -1)
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
        tb = localtime(time(0));
        char *ts = asctime(tb);
	this->writeLog(ts, strlen(ts));
	char *flag;
	if (type == 0)
		flag = "LOG:";
	else
		flag = "ERROR:";
	this->writeLog(flag, strlen(flag));
	this->writeLog(buff, len);	
}

static lrbLog *lrbLog::getInstance()
{
	static lrbLog lrblog;
	return &lrblog;
}

void lrbLog(const char *buff, size_t len) throw(lrbLogException) 
{
	lrbLog *instance = lrbLog::getInstance();
	instance->commitLog(buff, len, 0);	
}

void lrbError(const char *buff, size_t len) throw(lrbLogException)
{
	lrbLog *instance = lrbLog::getInstance();
	instance->commitLog(buff, len, 1);
}

