#ifndef _LRB_RUN_LOOP_H
#define _LRB_RUN_LOOP_H

#include <sys/time.h>
#include <functional>

namespace lrb {

	enum class RunLoopType {
		RLT_LOGIC = 0,		//必须放在开始
//		RLT_RENDER,

		RLT_NET,		// 必须倒数3位
		RLT_LOG,		// 必须倒数2位
		RLT_TIMER,		// 必须放在最后
		RLT_TOP
	};

	class RunLoop {
	public:
		static void initRunLoop(const std::function<void()> &func);
		// 不要在Net, Log 和 Timer 执行回调。因为有特定的调用接口
		static void runInLoop(const std::function<void()> &func, RunLoopType type, const timeval *tv = NULL);
		static RunLoopType loopType();
		static const char *loopName();
//		static void notifyLoop(RunLoopType type);
		static int addPollFd(int fd, short events, const std::function<void(int, short)> &func);
		static void updatePollFd(int handler, short events, const std::function<void(int, short)> &func);
		static void removePollFd(int handler);

//		RunLoop();
//		RunLoop(RunLoopType type);
//		~RunLoop();
		
	private:
	
		static bool execTask();
	
		static void timerFunc();
//		static void logFunc();
		static void loopFunc(RunLoopType type);
		static void startNewLoop(RunLoopType type);
		static void startTimerLoop();
//		static void startLogLoop();
		static void startLogicLoop(const std::function<void()> &func);
	
//		RunLoopType m_loopType;
	};

}


#endif
