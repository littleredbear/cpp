#include "../character/character.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd = open("file", O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd == -1) {
			printf("open fd error \n");
			return -1;
	}
	
	char buff[1024] = "alksdjfkjdkjskdjf";
	int len = strlen(buff);
	printf("len %d\n", len);
	int n = writen(fd, buff, strlen(buff));
	printf("write %d \n", n);
	close(fd);
}
