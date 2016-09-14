#include "MapConfig.h"
#include <iostream>

using namespace lrb;
using namespace cocos2d;
int main(int argc, char **argv)
{
	Point p = Point(100, 100);
	MapConfig  config(p);

	Point top = Point(10, 10);
	Point bot = Point(1,1);
	std::shared_ptr<Block> block(new Block(bot, top));

	Point t(12, 7);
	Point b(11, 3);
	std::shared_ptr<Block> block1(new Block(b, t));
	
	Point t2(15,15);
	Point b2(5,12);
	std::shared_ptr<Block> block2(new Block(b2,t2));
	
//	std::vector<std::shared_ptr<Block> > vec;
//	block->splitWithBlock(block1, vec);
	
//	std::shared_ptr<Block> cross = block->crossBlockWithBlock(block1);
//	if (cross)
//		std::cout << cross->blockDescribe() << std::endl;
//	else
//		std::cout << "no cross block" << std::endl;

//	std::cout << "----" << std::endl;

//	if (!vec.empty())
//		for (auto &iter : vec) 
//			std::cout << iter->blockDescribe() << std::endl;
//	else
//		std::cout << "no cross block" << std::endl;

	std::cout << "--------" << std::endl;

	block->useBlockId();
	block1->useBlockId();
	block2->useBlockId();
	config.addBlock(block);
	config.addBlock(block1);
	config.addBlock(block2);
	config.generateMap();
//	config.printMap();
	Point start(4, 13);
	Point end(16, 14);
	std::vector<Point> path = config.findPath(start, end, 2);
	std::cout << "----------" << std::endl;
	for (auto &p : path) 
		std::cout << p.x << ", " << p.y << std::endl;
	
//	Point bb1(10, 10);
//	Point tt1(15, 15);
//	std::shared_ptr<Block> tblock1(new Block(bb1, tt1));
//	
//	Point bb2(15, 12);
//	Point tt2(20, 20);
//	std::shared_ptr<Block> tblock2(new Block(bb2, tt2));
//	std::vector<Point> path = Block::inflexionPoints(tblock1, tblock2, 2);
//	if (!path.empty()) {
//		for (auto &p : path) 
//			std::cout << "x=" << p.x << ", y=" << p.y << std::endl;
//	} else {
//		std::cout << "inflexion empty" << std::endl;
//	}


}
