#include <vector>
#include <map>
#include <memory>
#include <string>
#include <list>
#include "Vec2.h"

#ifndef MAX
#define MAX(a,b) (((a) < (b)) ? (b) : (a))
#endif
#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

namespace lrb {

	class Block;
	typedef std::shared_ptr<Block> blockPtr;
	typedef std::vector<blockPtr> blockVec;

	class SearchNode {
	public:
		void *getData() {return m_data;};
	private:
		void *m_data;
	};

	class SearchTree {
	public:
		SearchTree();
		void addSearchNode(SearchNode *node);
		void removeSearchNode();
	private:
		SearchNode *m_root;
	};
	
//	class Point {
//	public:
//		Point(float x,  float y):m_x(x),m_y(y){};
//		float getX() {return m_x;};
//		float getY() {return m_y;};
//	private:
//		float m_x;
//		float m_y;
//	};
	
	class Block {
	public:
		Block(Point &bot, Point &top);
		int getBlockId() {return m_id;};
		void useBlockId();
		void splitWithBlock(std::shared_ptr<Block> &block, std::vector<std::shared_ptr<Block> > &vec);
		std::shared_ptr<Block> mergeWithBlock(std::shared_ptr<Block> &block);
		std::string blockDescribe();
		Point getBottom() {return m_bot;};
		Point getTop() {return m_top;};
		std::shared_ptr<Block> crossBlockWithBlock(std::shared_ptr<Block> &block);
		void clear() {m_frontVec.clear();};
		void addFrontBlock(blockPtr &block) {m_frontVec.push_back(block);};
		const blockVec &getFrontVec() {return m_frontVec;};

		static void relateBlockWithBlock(std::shared_ptr<Block> &block1, std::shared_ptr<Block> &block2);
		static std::vector<Point> inflexionPoints(std::shared_ptr<Block> &left, std::shared_ptr<Block> &right, float hw);
	private:
		Point m_bot;
		Point m_top;
		int m_id;
		blockVec m_frontVec;
	};

	class MapConfig {
	public:
		MapConfig(Point &p);
		void addBlock(std::shared_ptr<Block> &block);
		void destroyBlock(std::shared_ptr<Block> &block);
		void sortMove();
		void resetMap();
		void generateMap();
		void printMap();
		std::vector<Point> findPath(Point &start, Point &end, float width);
	private:
		void findNextBlock(blockPtr &startBlock, blockPtr &endBlock, Point &start, Point &end, float width);
		void generateWithBlock(std::shared_ptr<Block> &block);
		void mergeBlocksToMove(std::vector<std::shared_ptr<Block> > &vec);
		std::shared_ptr<Block> findBlock(Point &point);
		Point m_top;
		std::map<int, std::shared_ptr<Block> > m_blocks;
		std::list<std::shared_ptr<Block> >  m_move;
		std::map<float, blockVec > m_moveMap;
	};
}


