#include "lrbRunLoop.h"
#include <thread>


using namespace lrb;


TaskManager s_taskManager[(int)RunLoopType::RLT_TOP][(int)RunLoopType::RLT_TOP];
LoopPoller s_poller[(int)RunLoopType::RLT_TOP];


void RunLoop:initRunLoop(const std::function<void()> &func, RunLoopType type)
{
	runInLoop(func, type);
	
	
}

RunLoop::RunLoop():
m_loopType(RunLoopType::RLT_LOGIC)
{

}

RunLoop::RunLoop(RunLoopType type):
m_loopType(type)
{

}

RunLoop::~RunLoop()
{

}

void RunLoop::runInLoop(const std::function<void()> &func, RunLoopType type)
{
	if (func)
		s_taskManager[(int)type][(int)m_loopType].addTask(func);
}

bool RunLoop::execTask()
{
	bool ret = false;
	
	for (int t = 0; t < (int)RunLoopType::RLT_TOP; ++t)
		ret = s_taskManager[(int)m_loopType][t].execTask() || ret;
	
	return ret;
}


void RunLoop::loopFunc()
{
	do 
	{
		while(execTask());
		
		
		
	} while(1);
}

void RunLoop::startLoop()
{
	std::thread th(std::bind(&RunLoop::loopFunc, this));
	th.detach();
}




