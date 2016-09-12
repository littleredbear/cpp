#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include "Vec2.h"

class Block;
typedef std::vector<std::shared_ptr<Block> > blockVec;

class Block {
public:
	int m_id;
	void print() const {std::cout << "block" << std::endl;};

};
Block g_block;

const Block &getBlock()
{
	return g_block;
};

int main(int argc, char **argv)
{
	std::list<std::shared_ptr<Block> > list;
	std::shared_ptr<Block> b(new Block());
	b->m_id = 1;
	list.push_back(b);
	for (auto &iter : list) {
		iter->print();
	}
	std::shared_ptr<Block> b1(new Block());
	b1->m_id = 2;
	
	std::map<float, std::vector<std::shared_ptr<Block> > > mp;
	std::vector<std::shared_ptr<Block> > vec;
	vec.push_back(b);
	vec.push_back(b1);
	mp[1.1] = vec;

	for (auto &iter : mp) {
		blockVec &v = iter.second;
		std::sort(v.begin(), v.end(), [](std::shared_ptr<Block> &a, std::shared_ptr<Block> &b) {return a->m_id < b->m_id;});
	}

	std::map<float, std::vector<std::shared_ptr<Block> > >::iterator iter;
	iter = mp.find(1.1);
	if (iter != mp.end())  {
		for (auto it : iter->second) {
			std::cout << it->m_id << std::endl;
		}
		std::cout << "find" << std::endl;
	}
	std::cout << "---------" << std::endl;
	getBlock().print();

	std::shared_ptr<Block> &ttttt = vec.back();
	vec.pop_back();
	float lll = FLT_MAX;
//	ttttt->print();
	std::vector<int> tets;
	tets.push_back(11);
	tets.push_back(22);
	tets.push_back(33);
	std::sort(tets.begin(), tets.end(), [](int a, int b) {return a > b;});
	for (auto i : tets)
		std::cout << i << std::endl;

}
