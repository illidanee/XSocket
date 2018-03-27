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
	std::list<std::function<void()>> _Tasks;					//�����б�
	std::list<std::function<void()>> _TasksCache;				//���񻺳���
	std::mutex _TaskSCacheMutex;								//���񻺳�����

	bool _Run;													//��ǰ�߳��Ƿ�����
	XSignal _Signal;											//ͬ���ź�

private:
	int OnRun(int id);
};

#endif