#include "MapConfig.h"
#include <iostream>
#include <stdio.h>

using namespace lrb;

namespace {
	static int currentBlockId() {
		static int bid = 0;
		return ++bid;
	}
}

SearchTree::SearchTree():
m_root(NULL)
{

}

void SearchTree::addSearchNode(SearchNode *node)
{

}

Block::Block(Point &bot, Point &top):
m_bot(bot),
m_top(top),
m_id(-1)
{
}

void Block::useBlockId()
{
	if (m_id == -1) 
		m_id = currentBlockId();
}

std::string  Block::blockDescribe()
{
	char num[10];
	snprintf(num, 10, "%f", m_bot.x);
	std::string str = "[";
	str += num;
	str = str + ",";
	snprintf(num, 10, "%f", m_bot.y);
	str += num;
	str = str + ";";
	snprintf(num, 10, "%f", m_top.x);
	str += num;
	str = str + ",";
	snprintf(num, 10, "%f", m_top.y);
	str += num;
	str += "]";
	return str;
}

void Block::splitWithBlock(std::shared_ptr<Block> &block, std::vector<std::shared_ptr<Block> > &vec)
{
//	std::shared_ptr<Block> cross = this->crossBlockWithBlock(block);
//	if (!cross) {
//		return;
//	}
	Point bot = block->getBottom();
	Point top = block->getTop();
	if (m_bot.x < bot.x) {
		Point tp(bot.x, m_top.y);
		std::shared_ptr<Block> tmp(new Block(m_bot, tp));
		vec.push_back(tmp);
	}
	if (m_top.y > top.y) {
		Point bt(MAX(m_bot.x, bot.x), top.y);
		Point tp(MIN(m_top.x, top.x), m_top.y);
		std::shared_ptr<Block> tmp(new Block(bt, tp));
		vec.push_back(tmp);
	}
	if (m_top.x > top.x) {
		Point bt(top.x, m_bot.y);
		std::shared_ptr<Block> tmp(new Block(bt, m_top));
		vec.push_back(tmp);
	}
	if (m_bot.y < bot.y) {
		Point bt(MAX(m_bot.x, bot.x), m_bot.y);
		Point tp(MIN(m_top.x, top.x), bot.y);
		std::shared_ptr<Block> tmp(new Block(bt, tp));
		vec.push_back(tmp);
	}
}

std::shared_ptr<Block> Block::mergeWithBlock(std::shared_ptr<Block> &block)
{
	std::shared_ptr<Block> merge;
	Point bot = block->getBottom();
	Point top = block->getTop();

	if (m_bot.y == bot.y && m_top.y == top.y) {
		if (m_top.x == bot.x) {
			merge.reset(new Block(m_bot, top));
		} else if (top.x == m_bot.x) {
			merge.reset(new Block(bot, m_top));
		}
	}
	
	return merge;
}

std::shared_ptr<Block> Block::crossBlockWithBlock(std::shared_ptr<Block> &block)
{
	std::shared_ptr<Block> cross;
	float botx = MAX(m_bot.x, block->getBottom().x);
	float boty = MAX(m_bot.y, block->getBottom().y);
	float topx = MIN(m_top.x, block->getTop().x);
	float topy = MIN(m_top.y, block->getTop().y);
	
	if (botx < topx && boty < topy) {
		Point b(botx, boty);
		Point t(topx, topy);
		cross.reset(new Block(b, t));	
	}

	return cross;
}

void Block::relateBlockWithBlock(std::shared_ptr<Block> &block1, std::shared_ptr<Block> &block2)
{
	Point bot1 = block1->getBottom();
	Point top1 = block1->getTop();
	Point bot2 = block2->getBottom();
	Point top2 = block2->getTop();

	if (bot1.x == top2.x) {
		if ((bot1.y >= bot2.y && bot1.y <= top2.y) ||
		    (top1.y >= bot2.y && top1.y <= top2.y)) {
			block1->addFrontBlock(block2);
		}
	} else if (top1.x == bot2.x) {
		if ((bot2.y >= bot1.y && bot2.y <= top1.y) ||
		    (top2.y >= bot1.y && top2.y <= top1.y)) {
			block2->addFrontBlock(block1);
		}
	}
}

std::vector<Point> Block::inflexionPoints(std::shared_ptr<Block> &left, std::shared_ptr<Block> &right, float hw)
{
	std::vector<Point> vec;
	Point bot1 = left->getBottom();
	Point top1 = left->getTop();
	Point bot2 = right->getBottom();
	Point top2 = right->getTop();
	
	float boty = MAX(bot1.y, bot2.y);
	float topy = MIN(top1.y, top2.y);
	if (top1.x == bot2.x && topy-boty >= hw * 2) {
		if (bot1.y < bot2.y) {
			Point p(bot2.x-hw, boty+hw);
			vec.push_back(std::move(p));
		} else {
			Point p(bot2.x+hw, boty+hw);
			vec.push_back(std::move(p));
		}
		if (top1.y < top2.y) {
			Point p(top1.x+hw, topy-hw);
			vec.push_back(std::move(p));
		} else {
			Point p(top1.x-hw, topy-hw);
			vec.push_back(std::move(p));
		}
	}

	return vec;
}

MapConfig::MapConfig(Point &p):
m_top(p)
{
}

void MapConfig::addBlock(std::shared_ptr<Block> &block)
{
	if (block) {
		m_blocks[block->getBlockId()] = block;
	}
}

void MapConfig::mergeBlocksToMove(std::vector<std::shared_ptr<Block> > &vec)
{
	while (!vec.empty()) {
		std::shared_ptr<Block> block = vec.back();
		vec.pop_back();
		bool flag = true;
		for (auto iter=vec.begin();iter!=vec.end();++iter) {
			std::shared_ptr<Block> merge = block->mergeWithBlock(*iter);
			if (merge) {
				vec.erase(iter);
				vec.push_back(std::move(merge));
				flag = false;
				break;
			}
		}
		if (flag) {
			m_move.push_back(std::move(block));
		}
	}
}

void MapConfig::generateWithBlock(std::shared_ptr<Block> &block)
{
	std::vector<std::shared_ptr<Block> > vec;
//	std::cout << "generate block ->" << block->blockDescribe() << std::endl;
	
	for (auto iter=m_move.begin();iter != m_move.end();) {
		std::shared_ptr<Block> cross = (*iter)->crossBlockWithBlock(block);
		if (cross) {
			(*iter)->splitWithBlock(block, vec);
      			m_move.erase(iter++);
		} else {
			++iter;
		}
	}
	mergeBlocksToMove(vec);
}

void MapConfig::sortMove()
{
	for (auto &iter : m_move) {
		float x = iter->getBottom().x;
		std::map<float, blockVec>::iterator it;
		it = m_moveMap.find(x);
		if (it != m_moveMap.end()) {
			(it->second).push_back(iter);
		} else {
			blockVec vec;
			vec.push_back(iter);
			m_moveMap[x] = vec;
		}
	}

	for (auto &iter : m_moveMap) {
		blockVec &vec = iter.second;
		std::sort(vec.begin(), vec.end(), [](blockPtr &a, blockPtr &b) {
							return a->getTop().y > b->getTop().y;
						});
	}
	for (auto iter = m_move.begin();iter!=m_move.end();) {
		auto old = iter;
		for (auto iter1 = ++iter;iter1!=m_move.end();++iter1) {
			Block::relateBlockWithBlock(*old, *iter1);
		}
	}
}

void MapConfig::resetMap()
{
	for (auto &iter : m_move) 
		iter->clear();

	m_move.clear();
	Point bot(0, 0);
	std::shared_ptr<Block> block(new Block(bot, m_top));
	m_move.push_back(block);
	
	m_moveMap.clear();
}

void MapConfig::generateMap()
{
	resetMap();

	for (auto &iter : m_blocks) {
		generateWithBlock(iter.second);
	}
	sortMove();
}

void MapConfig::printMap()
{
	std::cout << "blocks: " << std::endl;
	for (auto &iter : m_blocks) {
		std::cout << "[id = " << iter.first << " " << iter.second->blockDescribe() <<  " ]" << std::endl;
	}
	std::cout << "moveArea: " << std::endl;
	for (auto &iter : m_moveMap) {
		for(auto &it : iter.second) {
			std::cout << "move->" << std::endl;
			std::cout << it->blockDescribe() << std::endl;
			const blockVec &vec = it->getFrontVec();
			std::cout << "front->" << std::endl;
			for (auto &f : vec) {
				std::cout << f->blockDescribe() << std::endl;
			}
		}
	}
}

std::shared_ptr<Block> MapConfig::findBlock(Point &point)
{
	for (auto &iter : m_moveMap) {
		if (iter.first < point.x) {
			for (auto &it : iter.second) {
				if (it->getTop().x > point.x && 
				    it->getTop().y > point.y && 
				    it->getBottom().y <= point.y) {
					return it;
				}
			}
		}
	}	

	blockPtr block;
	return block;
}

std::vector<Point> MapConfig::findPath(Point &start, Point &end, float width)
{
	std::vector<Point> path;
	std::shared_ptr<Block> startBlock = findBlock(start);
	if (startBlock) {
		std::cout << "start Block->" << startBlock->blockDescribe() << std::endl;
	} else {
		std::cout << "start Block not found!" << std::endl;
	}
	
	std::shared_ptr<Block> endBlock = findBlock(end);
	if (endBlock) {
		std::cout << "end Block->" << endBlock->blockDescribe() << std::endl;
	} else {
		std::cout << "end Block not found!" << std::endl;
	}
	
	findNextBlock(startBlock, endBlock, start, end, width);

	return path;
}

void MapConfig::findNextBlock(blockPtr &startBlock, endBlock &endBlock, Point &start, Point &end, float width)
{

}


