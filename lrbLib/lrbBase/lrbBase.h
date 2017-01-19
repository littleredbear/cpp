#ifndef _LRB_BASE_H
#define _LRB_BASE_H

#include <sys/time.h>

bool operator < (const struct timeval &tv1, const struct timeval &tv2)
{
	if (tv1.tv_sec == tv2.tv_sec)
		return tv1.tv_usec < tv2.tv_usec;

	return tv1.tv_sec < tv2.tv_sec;
}

#endif
