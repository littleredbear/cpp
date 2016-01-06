#ifndef LRB_H
#define LRB_H

#include "exception/lrbexception.h"
#include "character/lrbcharacter.h"
#include <sys/types.h>
#include <time.h>
#include <cstring>

enum lrbErrorNO {
	lrbOpenFileError = 1,
	lrbWriteFileError = 2
};

//#define LRBDEBUG
namespace {
	void lrbLog(const char *buff, size_t len) throw(lrbLogException);
	void lrbError(const char *buff, size_t len) throw(lrbLogException);
}

#endif
