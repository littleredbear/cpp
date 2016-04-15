#include <iostream>


class A {

	public:
		enum Level {
			Normal,
			High
		};


};

void test(A::Level lvl) 
{
	std::cout << lvl << std::endl;
}

int main (int argc, char **argv)
{

	std::cout << A::Normal << std::endl;
	test(A::High);	

}
