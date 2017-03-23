#include "lrbRunLoop.h"
#include <thread>
#include <assert.h>
#include <algorithm>
#include "lrbTask.h"
#include "lrbBase.h"
#include "lrbLogger.h"
#include "lrbLoopPoller.h"


using namespace lrb;

namespace {
	TaskManager s_taskManager[(int)RunLoopType::RLT_TOP-1][(int)RunLoopType::RLT_TOP];
	LoopPoller s_poller[(int)RunLoopType::RLT_TOP];
	TimerManager s_timerManager[(int)RunLoopType::RLT_TOP-1];

	__thread RunLoopType s_loopType = RunLoopType::RLT_LOGIC;
}

void RunLoop::initRunLoop()
{
	for (int i = 0; i < (int)RunLoopType::RLT_TOP - 1; ++i)
	{
		startNewLoop((RunLoopType)i);
	}
	
	startTimerLoop();
}

void RunLoop::initRunLoop(const std::function<void()> &func)
{
	assert(func);

	for (int i = 1; i < (int)RunLoopType::RLT_TOP-1; ++i) 
	{
		startNewLoop((RunLoopType)i);
	}

	startTimerLoop();
	
	startLogicLoop(func);

}


//RunLoop::RunLoop():
//m_loopType(RunLoopType::RLT_LOGIC)
//{
//
//}

//RunLoop::RunLoop(RunLoopType type):
//m_loopType(type)
//{
//
//}

//RunLoop::~RunLoop()
//{
//
//}

void RunLoop::runInLoop(const std::function<void()> &func, RunLoopType type, const timeval *tv)
{
	if (!func)
		return;

	if (tv != NULL)
	{
		s_timerManager[(int)type].addTask(func, tv);
		s_poller[(int)RunLoopType::RLT_TIMER].notify();
	} else {
		s_taskManager[(int)type][(int)loopType()].addTask(func);
		if (loopType() != type)
			s_poller[(int)type].notify();
	}
}

RunLoopType RunLoop::loopType()
{
	return s_loopType;
}

const char *RunLoop::loopName()
{
	switch(loopType())
	{
	case RunLoopType::RLT_LOGIC:
		return "LogicLoop";
	
	case RunLoopType::RLT_NET:
		return "NetLoop";

	case RunLoopType::RLT_LOG:
		return "LogLoop";
	
	case RunLoopType::RLT_TIMER:
		return "TimerLoop";

	default:
		return "";
	}
}

//void RunLoop::notifyLoop(RunLoopType type)
//{
//	if (s_loopType == type)
//		return;
	
//	s_poller[(int)type].notify();
//}

int RunLoop::addPollFd(int fd, short events, const std::function<void(int, short)> &func)
{
	return s_poller[(int)loopType()].addPollFd(fd, events, func);
}

void RunLoop::updatePollFd(int handler, short events, const std::function<void(int, short)> &func)
{
	s_poller[(int)loopType()].updatePollFd(handler, events, func);
}

void RunLoop::removePollFd(int handler)
{
	s_poller[(int)loopType()].removePollFd(handler);
}

void RunLoop::setLoopType(RunLoopType type)
{
	s_loopType = type;
}

bool RunLoop::execTask()
{
	bool ret = false;
	
	for (int t = 0; t < (int)RunLoopType::RLT_TOP; ++t)
		ret = s_taskManager[(int)loopType()][t].execTask() || ret;
	
	return ret;
}


void RunLoop::timerFunc()
{
	setLoopType(RunLoopType::RLT_TIMER);

	do 
	{
		const timeval *rtv = NULL;
		bool res = false;
		struct timeval ntv;

		do 
		{
			gettimeofday(&ntv, NULL);
			for (int t=0;t<(int)RunLoopType::RLT_TOP-1;++t) 
			{
				res = s_timerManager[t].sortTask() || res;
				const timeval *tmv = s_timerManager[t].execTask((RunLoopType)t, &ntv);
				if (tmv == NULL) 
					continue;
				
				if (rtv == NULL)
					rtv = tmv;
				else if ((*tmv) < (*rtv))
					rtv = tmv;
			}

		} while(res);
		
		int timeout = -1;	
		if (rtv != NULL) 
		{
			gettimeofday(&ntv, NULL);	
			if ((*rtv) < ntv)
				continue;
			else
				timeout = (std::min(2100000, (int)(rtv->tv_sec - ntv.tv_sec))) * 1000 + (int)(rtv->tv_usec - ntv.tv_usec) / 1000;
			if (timeout <= 0)
				continue;
		}
		
		s_poller[(int)loopType()].poll(timeout);
		
	} while(1);
}

//void RunLoop::logFunc()
//{
//	s_loopType = RunLoopType::RLT_LOG;
	
//	do 
//	{
//		Logger::flush();
//		s_poller[(int)s_loopType].poll();

//	}while(1);
//}

void RunLoop::loopFunc(RunLoopType type)
{
	setLoopType(type);

	Logger::initLogger();

	do 
	{
		while(execTask());
		
		s_poller[(int)loopType()].poll();
		
	} while(1);
}


void RunLoop::startNewLoop(RunLoopType type)
{
	std::thread th(std::bind(RunLoop::loopFunc, type));
	th.detach();
}

void RunLoop::startTimerLoop()
{
	std::thread th(std::bind(RunLoop::timerFunc));
	th.detach();
}

//void RunLoop::startLogLoop()
//{
//	std::thread th(std::bind(RunLoop::logFunc));
//	th.detach();
//}

void RunLoop::startLogicLoop(const std::function<void()> &func)
{
	setLoopType(RunLoopType::RLT_LOGIC);
	
	runInLoop(func, RunLoopType::RLT_LOGIC);
	loopFunc(RunLoopType::RLT_LOGIC);
}





