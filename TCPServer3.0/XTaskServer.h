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
	std::list<std::function<void()>> _Tasks;					//�����б�
	std::list<std::function<void()>> _TasksCache;				//���񻺳���
	std::mutex _TaskSCacheMutex;								//���񻺳�����

	void OnRun();

public:
	void Start();
	void AddTask(std::function<void()> pTask);
};

#endif