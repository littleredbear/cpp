#include "lrbTask.h"
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

using namespace lrb;


//-------------------------Task Node---------------------

TaskNode::TaskNode():
m_state(TaskState::TS_NONE),
m_next(NULL)
{

}

TaskNode::~TaskNode()
{

}

bool TaskNode::setTaskFunc(const std::function<void()> &func)
{
	if (m_state == TaskState::TS_TODO)
		return false;

	m_func = func;
	m_state = TaskState::TS_TODO;

	return true;
}

bool TaskNode::execTask()
{
	if (m_state == TaskState::TS_TODO && m_func) 
	{
		m_func();
		m_state = TaskState::TS_DONE;
		return true;
	}

	return false;
}

TaskNode *TaskNode::nextNode()
{
	if (m_next != NULL)
		return m_next;

	return this + 1;
}

void TaskNode::bindNextNode(TaskNode *next)
{
	m_next = next;
}

//-------------------Task Manager----------------

TaskManager::TaskManager():
m_addTask(NULL),
m_execTask(NULL),
m_size(s_defaultTaskNum)
{
	m_tasks[m_size-1].bindNextNode(&m_tasks[0]);
	m_addTask = &m_tasks[m_size-1];
	m_execTask = &m_tasks[0];
}

TaskManager::~TaskManager()
{
	for (auto ptr : m_ptrs)
		free(ptr);
}

void TaskManager::addTask(const std::function<void()> &func)
{
	if (m_addTask->nextNode()->setTaskFunc(func) == false) 
	{
		TaskNode *ptr = (TaskNode *)calloc(m_size, sizeof(TaskNode));
		if (ptr == NULL)
		{
			// to be continued
			return;
		}

		m_addTask->bindNextNode(ptr);
		ptr[m_size-1].bindNextNode(m_addTask->nextNode());
		ptr->setTaskFunc(func);
		m_addTask = ptr;

		m_ptrs.push_back(ptr);
		m_size = m_size << 1;
	} else {
		m_addTask = m_addTask->nextNode();
	}

}

bool TaskManager::execTask()
{
	
	if (m_execTask->execTask()) 
	{
		m_execTask = m_execTask->nextNode();
		return true;
	}

	return false;
}


//------------------------------Timer Task----------------------

TimerTask::TimerTask():
m_state(TaskState::TS_NONE),
m_next(NULL)
{

}

TimerTask::~TimerTask()
{

}

void TimerTask::bindNextTask(TimerTask *task)
{
	m_next = task;
}

bool TimerTask::setTimerTask(const std::function<void()> &func, const struct timeval *tv)
{
	assert(tv != NULL);

	if (m_state == TaskState::TS_TODO)
		return false;

	m_func = func;
	m_state = TaskState::TS_TODO;
	m_tv = *tv;

	return true;
}

const std::function<void()> &TimerTask::execFunc()
{
	return m_func;
}

const struct timeval &TimerTask::execTime()
{
	return m_tv;
}

TaskState TimerTask::taskState()
{
	return m_state;
}

void TimerTask::setDone()
{
	m_state = TaskState::TS_DONE;
}

TimerTask *TimerTask::nextTask()
{
	if (m_next != NULL)
		return m_next;

	return this + 1;
}

//---------------------Timer Manager------------------

TimerManager::TimerManager():
m_addTask(NULL),
m_execTask(NULL),
m_size(s_defaultNum)
{
	m_atasks[m_size-1].bindNextTask(&m_atasks[0]);
	m_addTask = &m_atasks[m_size-1];
	m_execTask = &m_atasks[0];
}

TimerManager::~TimerManager()
{
	for (auto ptr : m_ptrs)
		free(ptr);
}

void TimerManager::addTask(const std::function<void()> &func, const struct timeval *tv)
{
	if (m_addTask->nextTask()->setTimerTask(func, tv) == false) 
	{
		TimerTask *ptr = (TimerTask *)calloc(m_size, sizeof(TimerTask));
		if (ptr == NULL)
		{
			// to be continued
			return;
		}

		m_addTask->bindNextTask(ptr);
		ptr[m_size-1].bindNextTask(m_addTask->nextTask());
		ptr->setTimerTask(func, tv);
		m_addTask = ptr;

		m_ptrs.push_back(ptr);
		m_size = m_size << 1;
	} else {
		m_addTask = m_addTask->nextTask();
	}

}

bool TimerManager::sortTask()
{
	if (m_execTask->taskState() == TaskState::TS_TODO) 
	{
		m_tasks.insert(std::make_pair(m_execTask->execTime(), m_execTask->execFunc()));
		m_execTask = m_execTask->nextTask();
		return true;
	}

	return false;
}

const struct timeval *TimerManager::execTask(RunLoopType type, const struct timeval *tv)
{
	assert(tv != NULL);

	while(!m_tasks.empty()) 
	{
		auto iter = m_tasks.begin();
		if ((*tv) < iter->first)
			return &(iter->first);

		RunLoop::runInLoop(iter->second, type);
		m_tasks.erase(iter);
	}
	
	return NULL;
}











