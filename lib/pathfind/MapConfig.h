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

	class ChainInfo {
	public:
		ChainInfo():m_value(FLT_MAX),m_mark(false) {};
		const blockPtr & getFromBlock() {return m_from;};
		const float getValue() {return m_value;};
		blockVec &getPathBlocks() {return m_blocks;};
	
		cocos2d::Point getStartPoint() {return m_start;};
		void setStartPoint(cocos2d::Point p) {m_start = p;};
		cocos2d::Point getTopPoint() {return m_top;};
		void setTopPoint(cocos2d::Point p) {m_top = p;};
		cocos2d::Point getBotPoint() {return m_bot;};
		void setBotPoint(cocos2d::Point p) {m_bot = p;};

		void setFromBlock(blockPtr &block) {m_from = block;};
		void setValue(float value) {m_value = value;};
		bool getMark() {return m_mark;};
		void mark() {m_mark = true;};

		std::vector<cocos2d::Point> &getPath() {return m_path;};

		void reset() {m_from.reset();m_value=FLT_MAX;m_mark=false;m_path.clear();};
	private:
		blockPtr m_from;
		float m_value;
		bool m_mark;
		cocos2d::Point m_start;
		cocos2d::Point m_top;
		cocos2d::Point m_bot;
		std::vector<cocos2d::Point> m_path;
		blockVec m_blocks;
	};
	
	class Block {
	public:
		Block(cocos2d::Point &bot, cocos2d::Point &top);
		int getBlockId() {return m_id;};
		void useBlockId();
		void splitWithBlock(std::shared_ptr<Block> &block, std::vector<std::shared_ptr<Block> > &vec);
		std::shared_ptr<Block> mergeWithBlock(std::shared_ptr<Block> &block);
		std::string blockDescribe();
		cocos2d::Point getBottom() {return m_bot;};
		cocos2d::Point getTop() {return m_top;};
		std::shared_ptr<Block> crossBlockWithBlock(std::shared_ptr<Block> &block);
		void clear() {m_frontVec.clear();};
		void addFrontBlock(blockPtr &block) {m_frontVec.push_back(block);};
		blockVec &getFrontVec() {return m_frontVec;};
		std::shared_ptr<ChainInfo> &getChainInfo() {return m_chain;};

		static void relateBlockWithBlock(std::shared_ptr<Block> &block1, std::shared_ptr<Block> &block2);
		static std::vector<cocos2d::Point> inflexionPoints(std::shared_ptr<Block> &left, std::shared_ptr<Block> &right, float hw);
		static void checkMark(blockPtr &block, blockVec &vec);
	private:
		cocos2d::Point m_bot;
		cocos2d::Point m_top;
		int m_id;
		blockVec m_frontVec;
		std::shared_ptr<ChainInfo> m_chain;
	};

	class MapConfig {
	public:
		MapConfig(cocos2d::Point &p);
		void addBlock(std::shared_ptr<Block> &block);
		void destroyBlock(std::shared_ptr<Block> &block);
		void sortMove();
		void resetMap();
		void generateMap();
		void printMap();
		std::vector<cocos2d::Point> findPath(cocos2d::Point &start, cocos2d::Point &end, float width);
	private:
		void findRealPath(blockPtr &startBlock, blockPtr &endBlock, cocos2d::Point &end);
		float calculateRightPath(blockVec &vec, cocos2d::Point &start, cocos2d::Point &end, std::vector<cocos2d::Point> &linfs, bool last, std::shared_ptr<ChainInfo> &chain);
		float checkRightInside(blockVec &vec, cocos2d::Point &start, cocos2d::Point &end, bool last, std::shared_ptr<ChainInfo> &chain);
		bool checkRightBlock(blockPtr &startBlock, blockPtr &rightBlock, cocos2d::Point &end, bool last);
		float calculateLeftPath(blockVec &vec, cocos2d::Point &start, cocos2d::Point &end, std::vector<cocos2d::Point> &linfs, bool last, std::shared_ptr<ChainInfo> &chain);
		float checkLeftInside(blockVec &vec, cocos2d::Point &start, cocos2d::Point &end, bool last, std::shared_ptr<ChainInfo> &chain);
		bool checkLeftBlock(blockPtr &startBlock, blockPtr &leftBlock, cocos2d::Point &end, bool last);
		void generateWithBlock(std::shared_ptr<Block> &block);
		void mergeBlocksToMove(std::vector<std::shared_ptr<Block> > &vec);
		std::shared_ptr<Block> findBlock(cocos2d::Point &point);
		cocos2d::Point m_top;
		std::map<int, std::shared_ptr<Block> > m_blocks;
		std::list<std::shared_ptr<Block> >  m_move;
		std::map<float, blockVec > m_moveMap;
		float m_width;
	};
}


