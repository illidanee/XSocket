#include "XTask.h"

XTask::XTask()
{

}

XTask::~XTask()
{

}

void XTask::DoTask()
{

}

void XTaskServer::Start()
{
	std::thread t(std::mem_fn(&XTaskServer::OnRun), this);
	t.detach();
}

void XTaskServer::OnRun()
{
	while (true)
	{
		//�ӻ�������ȡ����
		if (!_TasksCache.empty())
		{
			std::lock_guard<std::mutex> lock(_TaskSCacheMutex);
			for (std::list<std::shared_ptr<XTask>>::iterator iter = _TasksCache.begin(); iter != _TasksCache.end(); ++iter)
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
		for (std::list<std::shared_ptr<XTask>>::iterator iter = _Tasks.begin(); iter != _Tasks.end(); ++iter)
		{
			(*iter)->DoTask();
			//delete *iter;
		}

		//�������
		_Tasks.clear();
	}


}

void XTaskServer::AddTask(std::shared_ptr<XTask>& pTask)
{
	std::lock_guard<std::mutex> lock(_TaskSCacheMutex);
	_TasksCache.push_back(pTask);
}