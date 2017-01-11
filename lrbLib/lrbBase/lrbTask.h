#ifndef _LRB_TASK_H
#define _LRB_TASK_H

#include <functional>
#include <vector>


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



//----------------------------Task Node Handler-------------------

	class TaskNodeHandler {
	public:
		TaskNodeHandler();
		TaskNodeHandler(TaskNode *node);
		~TaskNodeHandler();

		void bindTaskNode(TaskNode *node, uint32_t size);

		void addTask(const std::function<void()> &func);
		bool execTask();
		void next();

	private:
		TaskNode *m_taskNode;
		uint32_t m_size;
		std::vector<void *> m_ptrs;
		
	};


//----------------------------Task Manager--------------------------

	class TaskManager {
	public:
		const static int s_defaultTaskNum = 128;
		
		TaskManager();
		~TaskManager();

		void addTask(const std::function<void()> &func);
		bool execTask();
	
	private:
		TaskNode m_tasks[s_defaultTaskNum];

		TaskNodeHandler m_execHandler;
		TaskNodeHandler m_addHandler;

	};

}






#endif


