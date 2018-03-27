#ifndef __XTASKSERVER_H__
#define __XTASKSERVER_H__

#include "XCommon.h"
#include "XSignal.h"

#include <list>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>

class XTaskServer
{
public:
	XTaskServer();
	~XTaskServer();

	int Start(int id);
	int Stop(int id);

	void AddTask(std::function<void()> pTask);

private:									
	std::list<std::function<void()>> _Tasks;					//任务列表
	std::list<std::function<void()>> _TasksCache;				//任务缓冲区
	std::mutex _TaskSCacheMutex;								//任务缓冲区锁

	bool _Run;													//当前线程是否运行
	XSignal _Signal;											//同步信号

private:
	int OnRun(int id);
};

#endif