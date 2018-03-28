#ifndef __XTHREAD_H__
#define __XTHREAD_H__

#include "XSignal.h"
#include <functional>
#include <mutex>

class XThread
{
	typedef std::function<void(XThread*)> PThreadFunc;

public:
	XThread();
	void Start(PThreadFunc pBegin = nullptr, PThreadFunc pRun = nullptr, PThreadFunc pEnd = nullptr);
	void Stop();
	void Exit();

	bool IsRun();
private:
	void OnWork();

private:
	bool _Run;											//��ǰ�߳��Ƿ���������
	XSignal _Signal;									//ͬ���ź�
	std::mutex _Mutex;									//��ȫ��

	PThreadFunc _pBegin;
	PThreadFunc _pRun;
	PThreadFunc _pEnd;
};

#endif