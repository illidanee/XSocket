#include "XServer.h"

XServer::XServer(int id)
	:
	_ID(id),
	_pGlobalEventObj(nullptr),
	_LastTime(0),
	_ClientChange(true),
	_MaxSocketID(0)
{
}

XServer::~XServer()
{
}

void XServer::Init(XIGlobalEvent* pGlobalEventObj)
{
	//设置对象
	_pGlobalEventObj = pGlobalEventObj;
	//设置时间
	_LastTime = XTimer::GetTimeByMicroseconds();
}

void XServer::Done()
{
	_pGlobalEventObj = nullptr;
	_LastTime = 0;
}

void XServer::Start()
{
	XInfo("    XReceiveServer<ID=%d>:Start() Begin\n", _ID);

	//开启任务线程
	_TaskServer.Start(_ID);

	//开始服务线程
	_Thread.Start(
		nullptr,
		[this](XThread* pThread) {
		OnRun(pThread);
	},
		nullptr
		);

	XInfo("    XReceiveServer<ID=%d>:Start() End\n", _ID);
}

void XServer::Stop()
{
	XInfo("    XReceiveServer<ID=%d>:Stop() Begin\n", _ID);

	//关闭服务线程
	_Thread.Stop();

	//关闭任务线程
	_TaskServer.Stop(_ID);

	//关闭所有客户端连接
	_AllClients.clear();
	_AllClientsCache.clear();

	XInfo("    XReceiveServer<ID=%d>:Stop() End\n", _ID);
}

void XServer::AddClient(const std::shared_ptr<XClient> pClient)
{
	std::lock_guard<std::mutex> lock(_AllClientsCacheMutex);
	_AllClientsCache.insert(std::pair<SOCKET, std::shared_ptr<XClient>>(pClient->GetSocket(), pClient));
}

int XServer::GetClientNum()
{
	return (int)_AllClients.size() + (int)_AllClientsCache.size();
}

XIGlobalEvent* XServer::GetGlobalObj()
{
	return _pGlobalEventObj;
}

void XServer::AddTask(std::function<void()> pTask)
{
	_TaskServer.AddTask(pTask);
}

void XServer::OnRun(XThread* pThread)
{
	XInfo("    XReceiveServer<ID=%d>:OnRun() Begin\n", _ID);

	while (pThread->IsRun())
	{
		//是否有新客户端加入？
		if (!_AllClientsCache.empty())
		{
			std::lock_guard<std::mutex> lock(_AllClientsCacheMutex);
			for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClientsCache.begin(); iter != _AllClientsCache.end(); ++iter)
			{
				_AllClients.insert(std::pair<SOCKET, std::shared_ptr<XClient>>(iter->first, iter->second));
				if (_pGlobalEventObj)
					_pGlobalEventObj->OnClientJoin(iter->second.get());
			}
			_AllClientsCache.clear();

			_ClientChange = true;
		}

		//是否有客户端连接？
		if (_AllClients.empty())
		{
			//减少CPU消耗。
			std::this_thread::sleep_for(std::chrono::microseconds(1));

			continue;
		}

		//检查是否有客户端向服务器发送数据。
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

		//设置1毫秒间隔，可以提高客户端连接select效率。
		timeval tv = { 0, 1 };						//使用时间间隔可以提高客户端连接速度。使用阻塞模式更快。但此处不能使用组塞模式，需要执行定时检测任务。
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

void XServer::RecvData(fd_set& fdSet)
{
	//接收客户端数据
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
	{
		if (FD_ISSET(iter->first, &fdSet))
		{
			int ret = iter->second->RecvData();
			if (ret != 0)			//不等于0，ret为1说明接收缓冲区满了，主动断开连接。
			{
				if (_pGlobalEventObj)
					_pGlobalEventObj->OnClientLeave(iter->second.get());

				iter = _AllClients.erase(iter);
				_ClientChange = true;
				continue;
			}
		}

		++iter;
	}
}

void XServer::SendData(fd_set& fdSet)
{	
	//接收客户端数据
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
	{
		if (FD_ISSET(iter->first, &fdSet))
		{
			int ret = iter->second->SendData();
			if (ret < 0)			//小于0，ret为1时说明发送缓冲区为空，不应该断开。
			{
				if (_pGlobalEventObj)
					_pGlobalEventObj->OnClientLeave(iter->second.get());

				iter = _AllClients.erase(iter);
				_ClientChange = true;
				continue;
			}
		}

		++iter;
	}
}

void XServer::CheckTime()
{
	//循环计时。
	time_t curTime = XTimer::GetTimeByMicroseconds();
	time_t delta = curTime - _LastTime;
	_LastTime = curTime;

	//计时检测。
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); )
	{
		//心跳检测！
		if (iter->second->CheckHeartTime(delta))
		{
			if (_pGlobalEventObj)
				_pGlobalEventObj->OnClientLeave(iter->second.get());

			iter = _AllClients.erase(iter);
			_ClientChange = true;
			continue;
		}

		//计时发送检测！
		//iter->second->CheckSendTime(delta);

		++iter;
	}
}