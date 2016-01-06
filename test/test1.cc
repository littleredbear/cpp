
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <exception>
#include <iostream>

using namespace std;

class testException : public exception {
	private:
		const char *_msg;
	public:
		testException(const char *msg){this->_msg = msg;};
		const char * what() const throw() 
		{
			return this->_msg;
		}

};

class test {
	private:
	test() throw(testException &){throw testException("show test");};
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
	

	printf("end\n");

}
