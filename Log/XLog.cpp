#include "XLog.h"

XLog& XLog::GetInstance()
{
	static XLog log;
	return log;
}

void XLog::SetFile(const char* pFile, const char* pMode)
{
	XLog& log = GetInstance();

	if (log._File)
	{
		fclose(log._File);
		log._File = nullptr;
	}

	log._File = fopen(pFile, pMode);
	
	assert(log._File != nullptr);
}

XLog::XLog()
	:
	_File(nullptr),
	_Run(false),
	_WaitNum(0),
	_WakeNum(0)
{
	Start();
}

XLog::~XLog()
{
	Stop();

	if (_File)
		fclose(_File);
}

void XLog::Start()
{
	std::lock_guard<std::mutex> lock(_ThreadlMutex);

	if (!_Run)
	{
		//����״̬
		_Run = true;

		//��������������
		std::thread t(std::mem_fn(&XLog::Run), this);
		t.detach();	
	}
}

void XLog::Stop()
{
	std::lock_guard<std::mutex> lock(_ThreadlMutex);

	if (_Run)
	{
		//����״̬
		_Run = false;

		//�ź�-�ȴ�
		std::unique_lock<std::mutex> lock(_SignalMutex);
		if (--_WaitNum < 0)
		{
			_CV.wait(lock, [this]()->bool {
				return _WakeNum > 0;
			});
			--_WakeNum;
		}
	}
}

void XLog::Run()
{
	while (_Run)
	{
		//�ӻ�������ȡ����
		if (!_TasksCache.empty())
		{
			std::lock_guard<std::mutex> lock(_TasksCacheMutex);
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
		}

		//�������
		_Tasks.clear();
	}

	//�������е�ʣ�����ݡ�
	for (std::list<std::function<void()>>::iterator iter = _TasksCache.begin(); iter != _TasksCache.end(); ++iter)
	{
		(*iter)();
	}

	//�������
	_TasksCache.clear();

	//�ź�-����
	std::lock_guard<std::mutex> lock(_SignalMutex);
	if (++_WaitNum <= 0)
	{
		++_WakeNum;
		_CV.notify_one();
	}
}

void XLog::AddTask(std::function<void()> pTask)
{
	std::lock_guard<std::mutex> lock(_TasksCacheMutex);
	_TasksCache.push_back(pTask);
}