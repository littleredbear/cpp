#include "MapConfig.h"
#include <iostream>
#include <stdio.h>

using namespace lrb;
using namespace cocos2d;

namespace {
	static int currentBlockId() {
		static int bid = 0;
		return ++bid;
	}
}


Block::Block(Point &bot, Point &top):
m_bot(bot),
m_top(top),
m_id(-1),
m_chain(new ChainInfo())
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

void Block::checkMark(blockPtr &block, blockVec &vec) 
{
	auto &chainInfo = block->getChainInfo();
	if (!chainInfo->getMark()) {
		chainInfo->mark();
		vec.push_back(block);
	}
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

void MapConfig::findRealPath(blockPtr &startBlock, blockPtr &endBlock, Point &end)
{
	blockVec blocks;
	blocks.push_back(startBlock);
	auto &endChain = endBlock->getChainInfo();
	while(!blocks.empty() && !endChain->getMark()) {
		blockPtr next = blocks.back();
		blocks.pop_back();
		const blockPtr &fromBlock = next->getChainInfo()->getFromBlock();

		float nx = next->getTop().x;
		auto iter = m_moveMap.find(nx);
		if (iter != m_moveMap.end() && !(iter->second).empty()) {
			for (auto &rb : iter->second) {
				if (fromBlock == rb)
					continue;

				if (!checkRightBlock(next, rb, end, rb==endBlock))
					continue;

				Block::checkMark(rb, blocks);
				if (rb == endBlock) 
					break;
			}
		}

		if (!endChain->getMark()) {
			for (auto &lb : next->getFrontVec()) {
				if (fromBlock == lb)
					continue;

				if (!checkLeftBlock(next, lb, end, lb==endBlock))
					continue;

				Block::checkMark(lb, blocks);
				if (lb == endBlock)
					break;
			}
		}
		std::sort(blocks.begin(), blocks.end(), [](const blockPtr &a, const blockPtr &b) {
					return a->getChainInfo()->getValue() > b->getChainInfo()->getValue();});
	}
}

std::vector<Point> MapConfig::findPath(Point &start, Point &end, float width)
{
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
    m_width = width;
	if (startBlock == endBlock) {
		std::vector<Point> path;
		path.push_back(end);
		return path;
	} else {
		auto &chain = startBlock->getChainInfo();
		chain->setStartPoint(start);
		Point t(start.x, startBlock->getTop().y);
		Point b(start.x, startBlock->getBottom().y);
		chain->setTopPoint(t);
		chain->setBotPoint(b);
        chain->getPathBlocks().push_back(startBlock);
		findRealPath(startBlock, endBlock, end);
		return endBlock->getChainInfo()->getPath();
	}
}

float MapConfig::calculateRightPath(blockVec &vec, Point &start, Point &end, std::vector<Point> &linfs, bool last, std::shared_ptr<ChainInfo> &chain)
{
	auto iter = vec.begin();
	for (;iter!=vec.end();++iter) {
		if ((*iter)->getTop().x > start.x) {
			break;
		}
	}

	Point s2end = end-start;
	auto lastIter = vec.end() - 1;
	if (iter == lastIter || iter == vec.end()) {
		return s2end.getLength();
	}

	Point top, bot;
	Point s2top(0,1), s2bot(0,-1);
	for (auto it=iter;it!=lastIter-1;++it) {
		std::vector<Point> infs = Block::inflexionPoints(*it, *(it+1), m_width);
		Point s2infs1 = infs[1] - start;
		Point s2infs0 = infs[0] - start;
		if (s2infs1.cross(s2top) >= 0) {
			top = infs[1];
			s2top = s2infs1;
		}
		if (s2infs0.cross(s2bot) <= 0) {
			bot = infs[0];
			s2bot = s2infs0;
		}
	}

	Point s2linfs0 = linfs[0] - start;
	Point s2linfs1 = linfs[1] - start;
	
    if (!top.isZero()) {
        if (s2top.cross(s2linfs0) > 0) {
            (chain->getPath()).push_back(top);
            chain->setStartPoint(top);
            return s2top.getLength() + calculateRightPath(vec, top, end, linfs, last, chain);
        } else if (s2bot.cross(s2linfs1) < 0) {
            (chain->getPath()).push_back(bot);
            chain->setStartPoint(bot);
            return s2bot.getLength() + calculateRightPath(vec, bot, end, linfs, last, chain);
        }
        
        Point ltop = s2top.cross(s2linfs1) > 0 ? top : linfs[1];
        Point lbot = s2bot.cross(s2linfs0) < 0 ? bot : linfs[0];
        chain->setTopPoint(ltop);
        chain->setBotPoint(lbot);
    } else {
        chain->setTopPoint(linfs[1]);
        chain->setBotPoint(linfs[0]);
    }
    blockVec &bpath = chain->getPathBlocks();
    bpath.insert(bpath.begin(), iter, vec.end());
   
    return checkRightInside(vec, start, end, last, chain);
    
}

float MapConfig::checkRightInside(blockVec &vec, Point &start, Point &end, bool last, std::shared_ptr<ChainInfo> &chain)
{
	auto iter = vec.begin();
	for (;iter!=vec.end();++iter) {
		if ((*iter)->getTop().x > start.x) {
			break;
		}
	}

	Point s2end = end-start;
	auto lastIter = vec.end() - 1;
	if (iter == lastIter || iter == vec.end()) {
		return s2end.getLength();
	}

	Point top, bot;
	Point s2top(0,1), s2bot(0,-1);
	for (auto it=iter;it!=lastIter;++it) {
		std::vector<Point> infs = Block::inflexionPoints(*it, *(it+1), m_width);
		Point s2infs1 = infs[1] - start;
		Point s2infs0 = infs[0] - start;
		if (s2infs1.cross(s2top) >= 0) {
			top = infs[1];
			s2top = s2infs1;
		}
		if (s2infs0.cross(s2bot) <= 0) {
			bot = infs[0];
			s2bot = s2infs0;
		}
	}

	if (s2end.cross(s2top) < 0) {
		if (last) 
			(chain->getPath()).push_back(top);
		return s2top.getLength() + checkRightInside(vec, top, end, last, chain);
	} else if (s2end.cross(s2bot) > 0) {
		if (last)
			(chain->getPath()).push_back(bot);
		return s2bot.getLength() + checkRightInside(vec, bot, end, last, chain);
	} else {
		return s2end.getLength();
	}
}

bool MapConfig::checkRightBlock(blockPtr &startBlock, blockPtr &rightBlock, Point &end, bool last)
{
	std::vector<Point> infs = Block::inflexionPoints(startBlock, rightBlock, m_width);
	if (infs.size() != 2) {
		return false;
	}

	auto &startChain = startBlock->getChainInfo();
	Point start = startChain->getStartPoint();
	Point top = startChain->getTopPoint();
	Point bot = startChain->getBotPoint();

	blockVec vec = startChain->getPathBlocks();

	auto &rightChain = rightBlock->getChainInfo();
	std::shared_ptr<ChainInfo> chain(new ChainInfo());
    chain->setStartPoint(start);

	if (start.x > startBlock->getTop().x) {
		Point lend;
		if ((infs[0] - start).cross(top-start) > 0) {
			lend = infs[0];
		} else {
			lend = infs[1];
		}
        
		float value = checkLeftInside(vec, start, lend, true, chain);
		if (value < rightChain->getValue()) {
			rightChain->setValue(value);
			rightChain->setStartPoint(lend);
			Point lt(lend.x, lend.y+1);
			Point lb(lend.x, lend.y-1);
			rightChain->setTopPoint(lt);
			rightChain->setBotPoint(lb);
			rightChain->setFromBlock(startBlock);
			blockVec &bpath = rightChain->getPathBlocks();
			bpath.clear();
			if (lend.x > top.x) {
				bpath.push_back(rightBlock);
			} else {
				bpath.push_back(startBlock);
			}
			std::vector<Point> &rpath = rightChain->getPath();
			std::vector<Point> &cpath = chain->getPath();
			std::vector<Point> &spath = startChain->getPath();
			rpath.clear();
			rpath.insert(rpath.begin(), spath.begin(), spath.end());
			rpath.insert(rpath.end(), cpath.begin(), cpath.end());
		}
		return true;
	} else {
		vec.push_back(rightBlock);
	}

	float value = calculateRightPath(vec, start, end, infs, last, chain);
	if (value < rightChain->getValue()) {
		rightChain->setValue(value);
		rightChain->setStartPoint(chain->getStartPoint());
		rightChain->setTopPoint(chain->getTopPoint());
		rightChain->setBotPoint(chain->getBotPoint());
		rightChain->setFromBlock(startBlock);
		blockVec &bpath = rightChain->getPathBlocks();
        blockVec &chainBlocks = chain->getPathBlocks();
        bpath.swap(chainBlocks.empty() ? vec : chainBlocks);
		std::vector<Point> &rpath = rightChain->getPath();
		std::vector<Point> &cpath = chain->getPath();
		std::vector<Point> &spath = startChain->getPath();
		rpath.clear();
		rpath.insert(rpath.begin(), spath.begin(), spath.end());
		rpath.insert(rpath.end(), cpath.begin(), cpath.end());
	}
	
	return true;
}

float MapConfig::calculateLeftPath(blockVec &vec, Point &start, Point &end, std::vector<Point> &linfs, bool last, std::shared_ptr<ChainInfo> &chain)
{
	auto iter = vec.begin();
	for (;iter!=vec.end();++iter) {
		if ((*iter)->getBottom().x < start.x) {
			break;
		}
	}

	Point s2end = end-start;
	auto lastIter = vec.end() - 1;
	if (iter == lastIter || iter == vec.end()) {
		return s2end.getLength();
	}

	Point top, bot;
	Point s2top(0,1), s2bot(0,-1);
	for (auto it=iter;it!=lastIter-1;++it) {
		std::vector<Point> infs = Block::inflexionPoints(*it, *(it+1), m_width);
		Point s2infs1 = infs[1] - start;
		Point s2infs0 = infs[0] - start;
		if (s2infs1.cross(s2top) <= 0) {
			top = infs[1];
			s2top = s2infs1;
		}
		if (s2infs0.cross(s2bot) >= 0) {
			bot = infs[0];
			s2bot = s2infs0;
		}
	}

	Point s2linfs0 = linfs[0] - start;
	Point s2linfs1 = linfs[1] - start;
	
    if (!top.isZero()) {
        if (s2top.cross(s2linfs0) < 0) {
            (chain->getPath()).push_back(top);
            chain->setStartPoint(top);
            return s2top.getLength() + calculateLeftPath(vec, top, end, linfs, last, chain);
        } else if (s2bot.cross(s2linfs1) > 0) {
            (chain->getPath()).push_back(bot);
            chain->setStartPoint(bot);
            return s2bot.getLength() + calculateLeftPath(vec, bot, end, linfs, last, chain);
        }
        
        Point ltop = s2top.cross(s2linfs1) < 0 ? top : linfs[1];
        Point lbot = s2bot.cross(s2linfs0) > 0 ? bot : linfs[0];
        chain->setTopPoint(ltop);
        chain->setBotPoint(lbot);
    } else {
        chain->setTopPoint(linfs[1]);
        chain->setBotPoint(linfs[0]);
    }
		
    blockVec &bpath = chain->getPathBlocks();
    bpath.insert(bpath.begin(), iter, vec.end());
    return checkLeftInside(vec, start, end, last, chain);

}

float MapConfig::checkLeftInside(blockVec &vec, Point &start, Point &end, bool last, std::shared_ptr<ChainInfo> &chain)
{
	auto iter = vec.begin();
	for (;iter!=vec.end();++iter) {
		if ((*iter)->getBottom().x < start.x) {
			break;
		}
	}

	Point s2end = end-start;
	auto lastIter = vec.end() - 1;
	if (iter == lastIter || iter == vec.end()) {
		return s2end.getLength();
	}

	Point top, bot;
	Point s2top(0,1), s2bot(0,-1);
	for (auto it=iter;it!=lastIter;++it) {
		std::vector<Point> infs = Block::inflexionPoints(*(it+1), *it, m_width);
        
		Point s2infs1 = infs[1] - start;
		Point s2infs0 = infs[0] - start;
		if (s2infs1.cross(s2top) <= 0) {
			top = infs[1];
			s2top = s2infs1;
		}
		if (s2infs0.cross(s2bot) >= 0) {
			bot = infs[0];
			s2bot = s2infs0;
		}
	}

	if (s2end.cross(s2top) > 0) {
		if (last) 
			(chain->getPath()).push_back(top);
		return s2top.getLength() + checkLeftInside(vec, top, end, last, chain);
	} else if (s2end.cross(s2bot) < 0) {
		if (last)
			(chain->getPath()).push_back(bot);
		return s2bot.getLength() + checkLeftInside(vec, bot, end, last, chain);
	} else {
		return s2end.getLength();
	}
}

bool MapConfig::checkLeftBlock(blockPtr &startBlock, blockPtr &leftBlock, Point &end, bool last)
{
	std::vector<Point> infs = Block::inflexionPoints(leftBlock, startBlock, m_width);
	if (infs.size() != 2) {
		return false;
	}
	
	auto &startChain = startBlock->getChainInfo();
	Point start = startChain->getStartPoint();
	Point top = startChain->getTopPoint();
	Point bot = startChain->getBotPoint();

	blockVec vec = startChain->getPathBlocks();
	
	auto &leftChain = leftBlock->getChainInfo();
	std::shared_ptr<ChainInfo> chain(new ChainInfo());
    chain->setStartPoint(start);

	if (start.x < startBlock->getBottom().x) {
		Point lend;
		if ((infs[1] - start).cross(bot-start) > 0) {
			lend = infs[1];
		} else {
			lend = infs[0];
		}
        
		float value = checkRightInside(vec, start, lend, true, chain);
		if (value < leftChain->getValue()) {
			leftChain->setValue(value);
			leftChain->setStartPoint(lend);
			Point lt(lend.x, lend.y+1);
			Point lb(lend.x, lend.y-1);
			leftChain->setTopPoint(lt);
			leftChain->setBotPoint(lb);
			leftChain->setFromBlock(startBlock);
			blockVec &bpath = leftChain->getPathBlocks();
			bpath.clear();
			if (lend.x < bot.x) {
				bpath.push_back(leftBlock);
			} else {
				bpath.push_back(startBlock);
			}
			std::vector<Point> &rpath = leftChain->getPath();
			std::vector<Point> &cpath = chain->getPath();
			std::vector<Point> &spath = startChain->getPath();
			rpath.clear();
			rpath.insert(rpath.begin(), spath.begin(), spath.end());
			rpath.insert(rpath.end(), cpath.begin(), cpath.end());
		}
		return true;
	} else {
		vec.push_back(leftBlock);
	}

	float value = calculateLeftPath(vec, start, end, infs, last, chain);
	if (value < leftChain->getValue()) {
		leftChain->setValue(value);
		leftChain->setStartPoint(chain->getStartPoint());
		leftChain->setTopPoint(chain->getTopPoint());
		leftChain->setBotPoint(chain->getBotPoint());
		leftChain->setFromBlock(startBlock);
		blockVec &bpath = leftChain->getPathBlocks();
        blockVec &chainBlocks = chain->getPathBlocks();
        bpath.swap(chainBlocks.empty() ? vec : chainBlocks);
		std::vector<Point> &rpath = leftChain->getPath();
		std::vector<Point> &cpath = chain->getPath();
		std::vector<Point> &spath = startChain->getPath();
		rpath.clear();
		rpath.insert(rpath.begin(), spath.begin(), spath.end());
		rpath.insert(rpath.end(), cpath.begin(), cpath.end());
	}

	return true;
}





