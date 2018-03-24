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
	std::list<std::shared_ptr<XTask>> _Tasks;					//�����б�
	std::list<std::shared_ptr<XTask>> _TasksCache;				//���񻺳���
	std::mutex _TaskSCacheMutex;								//���񻺳�����

	void OnRun();

public:
	void Start();
	void AddTask(const std::shared_ptr<XTask>& pTask);
};

#endif