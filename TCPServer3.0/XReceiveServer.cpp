#include "XReceiveServer.h"

XReceiveServer::XReceiveServer(int id)
	:
	_ID(id),
	_pEventObj(nullptr),
	_LastTime(0),
	_ClientChange(true),
	_MaxSocketID(0),
	_Run(false)
{
}

XReceiveServer::~XReceiveServer()
{
}

int XReceiveServer::Init(XIEvent* pEventObj)
{
	//���ö���
	_pEventObj = pEventObj;
	//����ʱ��
	_LastTime = XTimer::GetTimeByMicroseconds();

	return 0;
}

int XReceiveServer::Done()
{
	_pEventObj = nullptr;
	_LastTime = 0;

	return 0;
}

int XReceiveServer::Start()
{
	XLog("XReceiveServer<ID=%d>:Start() Begin\n", _ID);

	_TaskServer.Start(_ID);

	_Run = true;
	std::thread t(std::mem_fn(&XReceiveServer::OnRun), this);
	t.detach();

	XLog("XReceiveServer<ID=%d>:Start() End\n", _ID);
	return 0;
}

int XReceiveServer::Stop()
{
	XLog("XReceiveServer<ID=%d>:Stop() Begin\n", _ID);

	//�رշ����߳�
	_Signal.Sleep();
	_Run = false;
	_Signal.Wait();

	//�ر���������߳�
	_TaskServer.Stop(_ID);

	//�ر����пͻ�������
	_AllClients.clear();
	_AllClientsCache.clear();

	XLog("XReceiveServer<ID=%d>:Stop() End\n", _ID);
	return 0;
}

void XReceiveServer::AddClient(const std::shared_ptr<XClient> pClient)
{
	std::lock_guard<std::mutex> lock(_AllClientsCacheMutex);
	_AllClientsCache.insert(std::pair<SOCKET, std::shared_ptr<XClient>>(pClient->GetSocket(), pClient));
}

int XReceiveServer::GetClientNum()
{
	return (int)_AllClients.size() + (int)_AllClientsCache.size();
}

void XReceiveServer::AddTask(std::function<void()> pTask)
{
	_TaskServer.AddTask(pTask);
}

int XReceiveServer::OnRun()
{
	XLog("XReceiveServer<ID=%d>:OnRun() Begin\n", _ID);
	while (_Run)
	{
		//ѭ����ʱ��
		time_t curTime = XTimer::GetTimeByMicroseconds();
		time_t delta = curTime - _LastTime;
		_LastTime = curTime;

		//��ʱ��⡣
		for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); )
		{
			//������⣡
			if (iter->second->CheckHeartTime(delta))
			{
				if (_pEventObj)
					_pEventObj->OnClientLeave(iter->second);

				iter = _AllClients.erase(iter);
				_ClientChange = true;
				continue;
			}

			//��ʱ���ͼ�⣡
			iter->second->CheckSendTime(delta);

			++iter;
		}

		//�Ƿ����¿ͻ��˼��룿
		if (!_AllClientsCache.empty())
		{
			std::lock_guard<std::mutex> lock(_AllClientsCacheMutex);
			for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClientsCache.begin(); iter != _AllClientsCache.end(); ++iter)
			{
				_AllClients.insert(std::pair<SOCKET, std::shared_ptr<XClient>>(iter->first, iter->second));
				if (_pEventObj)
					_pEventObj->OnClientJoin(iter->second);
			}
			_AllClientsCache.clear();

			_ClientChange = true;
		}

		//�Ƿ��пͻ������ӣ�
		if (_AllClients.empty())
		{
			//����CPU���ġ�
			std::this_thread::sleep_for(std::chrono::microseconds(1));

			continue;
		}

		//����Ƿ��пͻ�����������������ݡ�
		fd_set fdRead;
		FD_ZERO(&fdRead);

		if (_ClientChange)
		{
			_ClientChange = false;

			_MaxSocketID = 0;
			for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); ++iter)
			{
				FD_SET(iter->first, &fdRead);
				if (_MaxSocketID < iter->first)
					_MaxSocketID = iter->first;
			}

			memcpy(&_fdSetCache, &fdRead, sizeof(fd_set));
		}
		else
		{
			memcpy(&fdRead, &_fdSetCache, sizeof(fd_set));
		}

		//����10��������������߿ͻ�������selectЧ�ʡ�
		timeval tv = { 0, 1000 };			//ʹ��ʱ����������߿ͻ��������ٶȡ�ʹ������ģʽ���졣���˴�����ʹ������ģʽ����Ҫִ�ж�ʱ�������
		int ret = select((int)_MaxSocketID + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			XLog("Error:Select!\n");
			return -1;
		}
		else if (0 == ret)
		{
			continue;
		}

		//���տͻ�������
		for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
		{
			if (FD_ISSET(iter->first, &fdRead))
			{
				int ret = iter->second->RecvData();
				if (ret < 0)
				{
					if (_pEventObj)
						_pEventObj->OnClientLeave(iter->second);

					iter = _AllClients.erase(iter);
					_ClientChange = true;
					continue;
				}
			}

			++iter;
		}

	}

	XLog("XReceiveServer<ID=%d>:OnRun() End\n", _ID);
	_Signal.Wake();
	return 0;
}
