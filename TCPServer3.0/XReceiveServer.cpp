#include "XReceiveServer.h"

XReceiveServer::XReceiveServer(int id)
	:
	_ID(id),
	_pEventObj(nullptr),
	_LastTime(0),
	_ClientChange(true),
	_MaxSocketID(0)
{
}

XReceiveServer::~XReceiveServer()
{
}

void XReceiveServer::Init(XIEvent* pEventObj)
{
	//���ö���
	_pEventObj = pEventObj;
	//����ʱ��
	_LastTime = XTimer::GetTimeByMicroseconds();
}

void XReceiveServer::Done()
{
	_pEventObj = nullptr;
	_LastTime = 0;
}

void XReceiveServer::Start()
{
	XInfo("    XReceiveServer<ID=%d>:Start() Begin\n", _ID);

	//���������߳�
	_TaskServer.Start(_ID);

	//��ʼ�����߳�
	_Thread.Start(
		nullptr,
		[this](XThread* pThread) {
		OnRun(pThread);
	},
		nullptr
		);

	XInfo("    XReceiveServer<ID=%d>:Start() End\n", _ID);
}

void XReceiveServer::Stop()
{
	XInfo("    XReceiveServer<ID=%d>:Stop() Begin\n", _ID);

	//�رշ����߳�
	_Thread.Stop();

	//�ر������߳�
	_TaskServer.Stop(_ID);

	//�ر����пͻ�������
	_AllClients.clear();
	_AllClientsCache.clear();

	XInfo("    XReceiveServer<ID=%d>:Stop() End\n", _ID);
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

void XReceiveServer::OnRun(XThread* pThread)
{
	XInfo("    XReceiveServer<ID=%d>:OnRun() Begin\n", _ID);

	while (pThread->IsRun())
	{
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
		fd_set fdWrite;
		
		if (_ClientChange)
		{
			FD_ZERO(&fdRead);

			_MaxSocketID = 0;
			for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); ++iter)
			{
				FD_SET(iter->first, &fdRead);
				if (_MaxSocketID < iter->first)
					_MaxSocketID = iter->first;
			}

			memcpy(&_fdSetCache, &fdRead, sizeof(fd_set));

			_ClientChange = false;
		}
		else
		{
			memcpy(&fdRead, &_fdSetCache, sizeof(fd_set));
		}

		memcpy(&fdWrite, &fdRead, sizeof(fd_set));

		//����1��������������߿ͻ�������selectЧ�ʡ�
		timeval tv = { 0, 1 };						//ʹ��ʱ����������߿ͻ��������ٶȡ�ʹ������ģʽ���졣���˴�����ʹ������ģʽ����Ҫִ�ж�ʱ�������
		int ret = select((int)_MaxSocketID + 1, &fdRead, &fdWrite, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			XInfo("Error:Select!\n");
			pThread->Exit();
		}

		RecvData(fdRead);
		SendData(fdWrite);
		
		CheckTime();
	}

	XInfo("    XReceiveServer<ID=%d>:OnRun() End\n", _ID);
}

void XReceiveServer::RecvData(fd_set& fdSet)
{
	//���տͻ�������
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
	{
		if (FD_ISSET(iter->first, &fdSet))
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

void XReceiveServer::SendData(fd_set& fdSet)
{	
	//���տͻ�������
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
	{
		if (FD_ISSET(iter->first, &fdSet))
		{
			int ret = iter->second->SendData();
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

void XReceiveServer::CheckTime()
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
		//iter->second->CheckSendTime(delta);

		++iter;
	}
}