#include "lrbBase.h"

bool operator < (const timeval &tv1, const timeval &tv2)
{
	if (tv1.tv_sec == tv2.tv_sec)
		return tv1.tv_usec < tv2.tv_usec;

	return tv1.tv_sec < tv2.tv_sec;
}

