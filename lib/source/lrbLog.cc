
#include "lrbLog.h"

using namespace lrb;

lrbLog::lrbLog()
{
#ifdef LRBDEBUGLOG
	_fd = STDOUT_FILENO;
#else
	_fd = open(LRB_LOG_PATH, O_WRONLY|O_CREAT|O_APPEND,00644);
#endif
}

void lrbLog::writeLog(const char *buff, size_t len)
{
	writen(this->_fd, buff, len);
}

void lrbLog::commitLog(const char *buff, size_t len, char type)
{
	if (this->_fd == -1)
		return;

	struct tm *tb;
	time_t t = time(0);
	tb = localtime(&t);
	char *ts = asctime(tb);
	std::string msg;
	msg.append(ts);
	if (type == 0) {
		msg.append(" LOG: ");
	} else {
		msg.append(" ERROR: ");
	}
	msg.append(buff);
	msg.append("\n");
	writen(this->_fd, msg.c_str(), msg.length());
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


