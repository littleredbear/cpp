#include <math.h>
#include <iostream>
#include <memory>
#include <string>
#include <stdio.h>
#include <map>
#include <vector>
#include <functional>


class T {
public:
	T():m_a(0) {};
	T(int a):m_a(a){};
	~T(){std::cout << "test test" << m_a << std::endl;};
private:
	int m_a;
	int m_b;
};

void testMap(std::map<int, std::vector<std::shared_ptr<T> > > &map)
{
	std::vector<std::shared_ptr<T> > &vec = map[1];
	std::shared_ptr<T> t(new T(3));
	vec.push_back(t);	
	map.clear();
}

void testFunc(int a, int b)
{
	std::cout << a << "," << b << std::endl;
}

int main(int argc, char **argv)
{
//	std::map<int, std::vector<std::shared_ptr<T> > > map;
//	testMap(map);
//	std::cout << "end" << std::endl;
//	std::shared_ptr<T> a;
//	std::cout << sizeof(a) << std::endl;

//	uint32_t tt = -1;
//	std::cout << tt << std::endl;
//	std::vector<T> vec = map[1];
//	std::cout << vec.size() << std::endl;

	std::function<void(int, int)> f = std::bind(&testFunc, std::placeholders::_1, std::placeholders::_2);
	f(1, 3);
	
}
