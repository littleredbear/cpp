#ifndef _LRB_RUN_LOOP_H
#define _LRB_RUN_LOOP_H

#include <sys/time.h>
#include <functional>

namespace lrb {

	enum class RunLoopType {
		RLT_LOGIC = 0,		//必须放在开始
//		RLT_RENDER,

		RLT_NET,		
		RLT_LOG,		// 必须放在倒数2位
		RLT_TIMER,		// 必须放在最后
		RLT_TOP
	};

	class RunLoop {
	public:
		static void initRunLoop(const std::function<void()> &func);
		// type can't be RLT_TIMER
		static void runInLoop(const std::function<void()> &func, RunLoopType type, const timeval *tv = NULL);
		static RunLoopType loopType();
		static const char *loopName();
		static void notifyLoop(RunLoopType type);
		static void addPollFd(int fd, short events, const std::function<void(int, short)> &func);

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
	};

}


#endif
