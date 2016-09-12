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

	class ChainInfo {
	public:
		ChainInfo():m_value(FLT_MAX),m_mark(false) {};
		const blockPtr & getFromBlock() {return m_from;};
		const float getValue() {return m_value;};
		blockVec &getPathBlocks() {return m_blocks;};
	
		Point getStartPoint() {return m_start;};
		void setStartPoint(Point &p) {m_start = p;};
		Point getTopPoint() {return m_top;};
		void setTopPoint(Point &p) {m_top = p;};
		Point getBotPoint() {return m_bot;};
		void setBotPoint(Point &p) {m_bot = p;};

		void setFromBlock(blockPtr &block) {m_from = block;};
		void setValue(float value) {m_value = value;};
		bool getMark() {return m_mark;};
		void mark() {m_mark = true;};

		std::vector<Point> &getPath() {return m_path;};

		void reset() {m_from->reset();m_value=FLT_MAX;m_mark=false;m_path.clear();};
	private:
		blockPtr m_from;
		float m_value;
		bool m_mark;
		Point m_start;
		Point m_top;
		Point m_bot;
		std::vector<Point> m_path;
		blockVec m_blocks;
	};
	
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
		std::shared_ptr<ChainInfo> &getChainInfo() {return m_chain;};

		static void relateBlockWithBlock(std::shared_ptr<Block> &block1, std::shared_ptr<Block> &block2);
		static std::vector<Point> inflexionPoints(std::shared_ptr<Block> &left, std::shared_ptr<Block> &right, float hw);
		static void checkMark(blockPtr &block, blockVec &vec);
	private:
		Point m_bot;
		Point m_top;
		int m_id;
		blockVec m_frontVec;
		std::shared_ptr<ChainInfo> m_chain;
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
		std::vector<Point> findRealPath(blockPtr &startBlock, blockPtr &endBlock, Point &end, float width);
		std::vector<Point> pathFromBlock(blockPtr &endBlock);
		bool checkRightBlock(blockPtr &startBlock, blockPtr &endBlock, Point &end, float width);
		bool checkLeftBlock(blockPtr &startBlock, blockPtr &endBlock, Point &end, float width);
		void generateWithBlock(std::shared_ptr<Block> &block);
		void mergeBlocksToMove(std::vector<std::shared_ptr<Block> > &vec);
		std::shared_ptr<Block> findBlock(Point &point);
		Point m_top;
		std::map<int, std::shared_ptr<Block> > m_blocks;
		std::list<std::shared_ptr<Block> >  m_move;
		std::map<float, blockVec > m_moveMap;
	};
}


