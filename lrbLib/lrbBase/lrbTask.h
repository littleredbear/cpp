#ifndef _LRB_TASK_H
#define _LRB_TASK_H

#include <functional>
#include <vector>
#include "lrbRunLoop.h"
#include <map>
#include <sys/time.h>


namespace lrb {

	enum class TaskState {
		TS_NONE = 0,
		TS_TODO,
		TS_DONE
	};


//------------------------Task Node-------------------------

	class TaskNode {
	public:
		TaskNode();
		~TaskNode();

		bool setTaskFunc(const std::function<void()> &func);
		bool execTask();

		TaskNode *nextNode();
		void bindNextNode(TaskNode *next);
		
	private:
		std::function<void()> m_func;

		TaskState m_state;
		TaskNode *m_next;
	};

//----------------------------Task Manager-------------------

	class TaskManager {
	public:
		const static int s_defaultTaskNum = 128;

		TaskManager();
		~TaskManager();

		void addTask(const std::function<void()> &func);
		bool execTask();

	private:
		std::vector<void *> m_ptrs;
		TaskNode m_tasks[s_defaultTaskNum];

		TaskNode *m_addTask;
		TaskNode *m_execTask;
		uint32_t m_size;
		
	};

//--------------------------Timer Task---------------------------

	class TimerTask {
	public:
		TimerTask();
		~TimerTask();
		
		void bindNextTask(TimerTask *task);
		bool setTimerTask(const std::function<void()> &func, const timeval *tv);
		const std::function<void()> &execFunc();
		const timeval &execTime();
		
		TaskState taskState();
		void setDone();
		
		TimerTask *nextTask();
		
	private:
		std::function<void()> m_func;
		timeval m_tv;

		TaskState m_state;
		TimerTask *m_next;
	};

//-----------------------------Timer Manager------------------------

	class TimerManager {
	public:
		const static int s_defaultNum = 128;

		TimerManager();
		~TimerManager();

		void addTask(const std::function<void()> &func, const timeval *tv);
		bool sortTask();
		const timeval *execTask(RunLoopType type, const timeval *tv);

	private:
		std::vector<void *> m_ptrs;
		std::multimap<timeval, std::function<void()> > m_tasks;
		TimerTask m_atasks[s_defaultNum];

		TimerTask *m_addTask;
		TimerTask *m_execTask;
		uint32_t m_size;
	};

}






#endif


