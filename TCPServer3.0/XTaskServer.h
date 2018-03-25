#ifndef __XTASKSERVER_H__
#define __XTASKSERVER_H__

#include <list>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>

class XTaskServer
{
private:
	std::list<std::function<void()>> _Tasks;					//任务列表
	std::list<std::function<void()>> _TasksCache;				//任务缓冲区
	std::mutex _TaskSCacheMutex;								//任务缓冲区锁

	void OnRun();

public:
	void Start();
	void AddTask(std::function<void()> pTask);
};

#endif