#include "XTaskServer.h"

XTaskServer::XTaskServer()
{
}

XTaskServer::~XTaskServer()
{
}

int XTaskServer::Start(int id)
{
	XLog("XTaskServer<ID=%d>:Start() Begin\n", id);

	//���������߳�
	_Thread.Start(
		nullptr, 
		[this](XThread* pThread) {
			OnRun(pThread);
		}, 
		nullptr);

	XLog("XTaskServer<ID=%d>:Start() End\n", id);
	return 0;
}

int XTaskServer::Stop(int id)
{
	XLog("XTaskServer<ID=%d>:Stop() Begin\n", id);

	//�رշ����߳�
	_Thread.Stop();

	XLog("XTaskServer<ID=%d>:Stop() End\n", id);
	return 0;
}

int XTaskServer::OnRun(XThread* pThread)
{
	XLog("XTaskServer<ID=%d>:OnRun() Begin\n", -1);

	while (pThread->IsRun())
	{
		//�ӻ�������ȡ����
		if (!_TasksCache.empty())
		{
			std::lock_guard<std::mutex> lock(_TaskSCacheMutex);
			for (std::list<std::function<void()>>::iterator iter = _TasksCache.begin(); iter != _TasksCache.end(); ++iter)
			{
				_Tasks.push_back(*iter);
			}
			_TasksCache.clear();
		}

		//û�����ݡ�
		if (_Tasks.empty())
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			continue;
		}

		//��������
		for (std::list<std::function<void()>>::iterator iter = _Tasks.begin(); iter != _Tasks.end(); ++iter)
		{
			(*iter)();
			//delete *iter;
		}

		//�������
		_Tasks.clear();
	}

	XLog("XTaskServer<ID=%d>:OnRun() End\n", -1);

	return 0;
}

void XTaskServer::AddTask(std::function<void()> pTask)
{
	std::lock_guard<std::mutex> lock(_TaskSCacheMutex);
	_TasksCache.push_back(pTask);
}