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

#define LRBDEBUGLOG
namespace lrb {
	extern void LrbLOG(const char *buff, size_t len) throw(lrb::lrbLogException);
	extern void LrbERROR(const char *buff, size_t len) throw(lrb::lrbLogException);
}

#endif
