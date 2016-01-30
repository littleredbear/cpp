
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <exception>
#include <stdlib.h>
#include <iostream>

using namespace std;

class testException {
	private:
		const char *_msg;
	public:
		testException(const char *msg){this->_msg = msg;};
		const char * what() const throw() 
		{
			return this->_msg;
		}

};

class test1 : public testException {
	public:
		test1(const char *msg):testException(msg){};
};

class test {
	private:
	test() throw(testException &){throw test1("show test");};
	public:
	static test* getInstance(){static test t;return &t;};
	void show() {cout << "show" << endl;};
};

int main(int argc, char **argv) 
{
/*
	char buf[] = "test good\n";
	int size = sizeof(buf)/sizeof(buf[0]);
	write(STDOUT_FILENO,buf,size);	
*/
	
	int fd = open("/dev/null", O_RDWR);
	try {
		test::getInstance()->show();

	} catch (testException e) {

		cout << e.what() << endl;
	} catch (testException &e) {
	
		cout << "reference" << e.what() << endl;
	} catch (testException *e) {
		cout << "pointer " << e->what() << endl;
		delete e;
	}
/*	
	char *ptr = (char *)calloc(2, sizeof(int));
	printf("ptr:%s\n",ptr);
	printf("ptr:%d\n",ptr);
	free(ptr);
	printf("ptr:%d\n",ptr);
	char *p = NULL;
	free(p);

	printf("end\n");
*/

}
