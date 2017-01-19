#ifndef _LRB_RUN_LOOP_H
#define _LRB_RUN_LOOP_H

#include "lrbLoopPoller.h"
#include <sys/time.h>

namespace lrb {

#define LRBTIMERLOOP

	enum class RunLoopType {
		RLT_LOGIC = 0,		//必须放在开始
//		RLT_RENDER,
//		RLT_NET,
#ifdef LRBTIMERLOOP
		RLT_TIMER,		// 必须放在最后
#endif
		RLT_TOP
	};


#ifdef LRBTIMERLOOP
	#define LOOPLEN (int)RunLoopType::RLT_TOP - 1
	class TimerManager;
#else
	#define LOOPLEN (int)RunLoopType::RLT_TOP
#endif
	class TaskManager;
	class RunLoop {
	public:
		static void initRunLoop(const std::function<void()> &func);
		static void runInLoop(const std::function<void()> &func, RunLoopType type, const timeval *tv = NULL);

//		RunLoop();
//		RunLoop(RunLoopType type);
//		~RunLoop();
		
	private:
	
		static bool execTask();
	
		static void timerFunc();
		static void loopFunc(RunLoopType type);
		static void startNewLoop(RunLoopType type);
		static void startTimerLoop();
		static void startLogicLoop(const std::function<void()> &func);
	
//		RunLoopType m_loopType;

		static TaskManager s_taskManager[LOOPLEN][(int)RunLoopType::RLT_TOP];
		static LoopPoller s_poller[(int)RunLoopType::RLT_TOP];
#ifdef LRBTIMERLOOP
		static TimerManager s_timerManager[(int)RunLoopType::RLT_TOP-1];
#endif
	};

}


#endif
