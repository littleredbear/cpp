#include <stdio.h>
#include <iostream>
#include "../lib/include/lrbLog.h"
#include <cstring>
#include <string>
#include <pthread.h>

using namespace lrb;

class test1 {
	public:
	int _b;

};

class test {
	public:
		int _a;
		test1 _t1;

	public:
	test(int a):_a(a) {};
	test(test1 &t1):_t1(t1) {};
	void testT();
	static void *threadT(void *arg);
	void testA(int a, int b=1, int c= 2);
};

void test::testT()
{
	pthread_t ntid;
	int n =	pthread_create(&ntid, NULL, threadT, NULL); 
	if (n != 0)
		printf("create thread error!\n");	
	printf("main thread\n");
}

void test::testA(int a, int b, int c)
{
	int sum = a + b + c;
	printf("sum:%d\n", sum);

}
	
void *test::threadT(void *arg)
{
	printf("child thread\n");
	return 0;
}

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

	test t(2);
	t.testA(1);	
	t.testA(1,10,1);
	test t1 = 1;
	t1 = t;
	
	test1 t11;
	t11._b = 3;
	test t12 = t11;
//	t12 = t;
	printf("t:%d,%d,%d,%d", t11._b, t1._t1._b, t12._t1._b, t12._a); 
	
}
