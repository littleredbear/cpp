#include "base/lrbCharacter.h"

ssize_t readn(int fd, void *buff, size_t n) 
{
	ssize_t nread;
	size_t nleft = n;;
	char *ptr = (char *)buff;

	while(nleft > 0) {
			if ((nread = read(fd, ptr, nleft)) < 0) {
					if (errno == EINTR) {
							nread = 0;
					} else {
							return -1;
					}
			} else if (nread == 0) {
					break;
			}

			nleft -= nread;
			ptr += nread;
	}

	return n - nleft;
}

ssize_t writen(int fd, const void *vptr, size_t n) {

		size_t nleft = n;
		ssize_t nwritten;
		const char *ptr = (const char *)vptr;

		while (nleft > 0) {
				if ((nwritten = write(fd, ptr, nleft)) <= 0) {
						if (nwritten < 0 && errno == EINTR) {
								nwritten = 0;
						} else {
								return -1;
						}
				}

				nleft -= nwritten;
				ptr += nwritten;
		}

		return n;		
}


