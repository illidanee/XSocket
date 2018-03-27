#include "XTaskServer.h"

XTaskServer::XTaskServer()
	:
	_Run(false)
{
}

XTaskServer::~XTaskServer()
{
}

int XTaskServer::Start(int id)
{
	XLog("XTaskServer<ID=%d>:Start() Begin\n", id);

	//���������߳�
	_Run = true;
	std::thread t(std::mem_fn(&XTaskServer::OnRun), this, id);
	t.detach();

	XLog("XTaskServer<ID=%d>:Start() End\n", id);
	return 0;
}

int XTaskServer::Stop(int id)
{
	XLog("XTaskServer<ID=%d>:Stop() Begin\n", id);

	//�رշ����߳�
	_Run = false;
	_Signal.Wait();

	//�ر����� - ѭ��ִ����û�������ˡ�
	//_Tasks.clear();
	//_TasksCache.clear();

	XLog("XTaskServer<ID=%d>:Stop() End\n", id);
	return 0;
}

int XTaskServer::OnRun(int id)
{
	XLog("XTaskServer<ID=%d>:OnRun() Begin\n", id);

	while (_Run)
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

	XLog("XTaskServer<ID=%d>:OnRun() End\n", id);
	_Signal.Wake();
	return 0;
}

void XTaskServer::AddTask(std::function<void()> pTask)
{
	std::lock_guard<std::mutex> lock(_TaskSCacheMutex);
	_TasksCache.push_back(pTask);
}