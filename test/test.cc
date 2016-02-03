#include <stdio.h>
#include <iostream>
#include "../lib/include/lrbLog.h"
#include <cstring>
#include <string>

using namespace lrb;
int main(int argc, char **argv)
{
	int fd = open("file", O_RDWR | O_APPEND | O_CREAT, 00644);
	if (fd == -1) {
			printf("open fd error \n");
			return -1;
	}
	
	char buff[1024] = "alksdjfkjdkjskdjf";
	int len = strlen(buff);
	printf("len %d\n", len);
	close(fd);
	
	char *a = NULL;
	char b[] = "dk";
	a = b;
	int n = strcmp(a, b);
	printf("c:%d\n", n);
	std::string ad;
	ad = ad + "kfjd";
	printf("%s\n", ad.c_str());
}
