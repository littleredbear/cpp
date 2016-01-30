#ifndef LRB_H
#define LRB_H

#include "lrbException.h"
#include "lrbCharacter.h"
#include <sys/types.h>
#include <time.h>
#include <cstring>
#include <stdlib.h>

enum lrbErrorNO {
	lrbOpenFileError = 1,
	lrbWriteFileError = 2
};

//#define LRBDEBUGLOG
namespace lrb {
	extern void LrbLOG(const char *buff, size_t len);
	extern void LrbERROR(const char *buff, size_t len);
}

#endif
