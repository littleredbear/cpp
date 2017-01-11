#include "lrbTask.h"
#include <assert.h>
#include <stdlib.h>

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


//-------------------Task Node Handler----------------

TaskNodeHandler::TaskNodeHandler():
m_taskNode(NULL),
m_size(0)
{

}


TaskNodeHandler::TaskNodeHandler(TaskNode *node):
m_taskNode(node)
{

}

TaskNodeHandler::~TaskNodeHandler()
{
	for (auto ptr : m_ptrs)
		free(ptr);
}

void TaskNodeHandler::bindTaskNode(TaskNode *node, uint32_t size)
{
	m_taskNode = node;
	m_size = size;
}

void TaskNodeHandler::addTask(const std::function<void()> &func)
{
	assert(m_taskNode != NULL && m_size > 0);
	
	if (m_taskNode->nextNode()->setTaskFunc(func) == false) 
	{
		TaskNode *ptr = (TaskNode *)calloc(m_size, sizeof(TaskNode));
		if (ptr == NULL)
		{
			// to be continued
			return;
		}

		m_taskNode->bindNextNode(ptr);
		ptr[m_size-1].bindNextNode(m_taskNode->nextNode());
		ptr->setTaskFunc(func);
		m_taskNode = ptr;

		m_ptrs.push_back(ptr);
		m_size = m_size << 1;
	} else {
		m_taskNode = m_taskNode->nextNode();
	}
}

bool TaskNodeHandler::execTask()
{
	assert(m_taskNode != NULL);
	
	if (m_taskNode->execTask()) 
	{
		m_taskNode = m_taskNode->nextNode();
		return true;
	}

	return false;
}


//---------------------Task Manager-------------------

TaskManager::TaskManager()
{
	m_tasks[s_defaultTaskNum-1].bindNextNode(&m_tasks[0]);
	m_execHandler.bindTaskNode(&m_tasks[0], s_defaultTaskNum);
	m_addHandler.bindTaskNode(&m_tasks[s_defaultTaskNum-1], s_defaultTaskNum);
}


TaskManager::~TaskManager()
{

}

void TaskManager::addTask(const std::function<void()> &func)
{
	m_addHandler.addTask(func);
}

bool TaskManager::execTask()
{
	return m_execHandler.execTask();
}






