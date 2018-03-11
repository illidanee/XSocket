#ifndef __XTASK_H__
#define __XTASK_H__

#include <mutex>
#include <list>
#include <chrono>
#include <thread>

class XTask
{
public:
	XTask();
	virtual ~XTask();

	virtual void DoTask();
};

class XTaskServer
{
private:
	std::list<XTask*> _Tasks;					//�����б�
	std::list<XTask*> _TasksCache;				//���񻺳���
	std::mutex _TaskSCacheMutex;				//���񻺳�����

	void OnRun();

public:
	void Start();
	void AddTask(XTask* pTask);
};

#endif // 
