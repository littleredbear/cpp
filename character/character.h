#ifndef __LRB_CHARACTER_H
#define __LRB_CHARACTER_H

#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

ssize_t readn(int filedes, void *buff, size_t nbytes);
ssize_t writen(int filedes, const void *buff, size_t nbytes);
ssize_t readline(int filedes, void *buff, size_t maxlen);


#endif
