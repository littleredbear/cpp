#ifndef _LRB_RUN_LOOP_H
#define _LRB_RUN_LOOP_H

#include "lrbTask.h"
#include "lrbLoopPoller.h"

namespace lrb {

	enum class RunLoopType {
		RLT_LOGIC = 0,
//		RLT_RENDER,
//		RLT_NET,
		RLT_TIMER,
		RLT_TOP
	};

	class RunLoop {
	public:
		static void initRunLoop(const std::function<void()> &func, RunLoopType type);
		
		RunLoop();
		RunLoop(RunLoopType type);
		~RunLoop();
		
		void runInLoop(const std::function<void()> &func, RunLoopType type);
		bool execTask();
	
		void loopFunc();
		void startLoop();
		
	private:
		RunLoopType m_loopType;

		static TaskManager s_taskManager[(int)RunLoopType::RLT_TOP][(int)RunLoopType::RLT_TOP];
		static LoopPoller s_poller[(int)RunLoopType::RLT_TOP];
	};

}


#endif
