#ifndef __XTASKSERVER_H__
#define __XTASKSERVER_H__

#include <list>
#include <mutex>
#include <thread>
#include <chrono>

#include "XTask.h"

class XTaskServer
{
private:
	std::list<std::shared_ptr<XTask>> _Tasks;					//任务列表
	std::list<std::shared_ptr<XTask>> _TasksCache;				//任务缓冲区
	std::mutex _TaskSCacheMutex;								//任务缓冲区锁

	void OnRun();

public:
	void Start();
	void AddTask(const std::shared_ptr<XTask>& pTask);
};

#endif