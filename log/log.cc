
#include "log.h"

using namespace lrb;

lrbLog::lrbLog()
{
#ifdef LRBDEBUGLOG
	_fd = STDOUT_FILENO;
#else
	_fd = open("log.txt", O_WRONLY|OCREAT|O_APPEND,00644);
#endif
}




