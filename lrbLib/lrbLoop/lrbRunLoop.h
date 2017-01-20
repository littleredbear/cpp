#ifndef _LRB_RUN_LOOP_H
#define _LRB_RUN_LOOP_H

#include "lrbLoopPoller.h"
#include <sys/time.h>

namespace lrb {

	enum class RunLoopType {
		RLT_LOGIC = 0,		//必须放在开始
//		RLT_RENDER,
//		RLT_NET,

		RLT_LOG,		// 必须放在倒数2位
		RLT_TIMER,		// 必须放在最后
		RLT_TOP
	};


#define LOOPLEN (int)RunLoopType::RLT_TOP - 1
	class TimerManager;
	class TaskManager;

	class RunLoop {
	public:
		static void initRunLoop(const std::function<void()> &func);
		// type can't be RLT_TIMER
		static void runInLoop(const std::function<void()> &func, RunLoopType type, const timeval *tv = NULL);
		static RunLoopType loopType();
		static const char *loopName();
		static void notifyLoop(RunLoopType type);

//		RunLoop();
//		RunLoop(RunLoopType type);
//		~RunLoop();
		
	private:
	
		static bool execTask();
	
		static void timerFunc();
		static void logFunc();
		static void loopFunc(RunLoopType type);
		static void startNewLoop(RunLoopType type);
		static void startTimerLoop();
		static void startLogLoop();
		static void startLogicLoop(const std::function<void()> &func);
	
//		RunLoopType m_loopType;

		static TaskManager s_taskManager[LOOPLEN][(int)RunLoopType::RLT_TOP];
		static LoopPoller s_poller[(int)RunLoopType::RLT_TOP];
		static TimerManager s_timerManager[(int)RunLoopType::RLT_TOP-1];

	};

}


#endif
