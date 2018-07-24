#include "XServer.h"



void XServer::AddTask(std::function<void()> pTask)
{
	_TaskServer.AddTask(pTask);
}

XServer::XServer()
	:
	_pGlobalEventObj(nullptr),
	_ID(0),
	_LastFrameTime(0),
	_CurFrameTime(0),
	_FrameTimeDelta(0),
	_ClientChange(false),
	_MaxSocketID(0)
{
}

XServer::~XServer()
{
}

void XServer::Init(XIGlobalEvent* pGlobalEventObj, int id)
{
	//设置全局事件对象
	_pGlobalEventObj = pGlobalEventObj;
	//设置服务线程ID
	_ID = id;

	//设置时间
	_LastFrameTime = XTimer::GetTimeByMicroseconds();
	_CurFrameTime = 0;
	_FrameTimeDelta = 0;

	//其他
	FD_ZERO(&_FdRead);
	FD_ZERO(&_FdWrite);
	FD_ZERO(&_FdSetCache);
	_ClientChange = false;
	_MaxSocketID = 0;
}

void XServer::Done()
{
	////设置全局事件对象
	//_pGlobalEventObj = nullptr;
	////设置服务线程ID
	//_ID = 0;

	////设置时间
	//_LastFrameTime = XTimer::GetTimeByMicroseconds();
	//_CurFrameTime = 0;
	//_FrameTimeDelta = 0;

	////其他
	//FD_ZERO(&_FdRead);
	//FD_ZERO(&_FdWrite);
	//FD_ZERO(&_FdSetCache);
	//_ClientChange = false;
	//_MaxSocketID = 0;
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

XIGlobalEvent* XServer::GetGlobalObj()
{
	return _pGlobalEventObj;
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

void XServer::OnRun(XThread* pThread)
{
	XInfo("    XReceiveServer<ID=%d>:OnRun() Begin\n", _ID);

	while (pThread->IsRun())
	{
		//必须在Continue之前，负责没有客户连接时会累积时间长度delta。
		UpdateFrameTimeDelta();

		int ret = 0;

		//检测是否有新的客户端以及当前是否有客户端。
		ret = CheckClient();
		if (ret < 0)
		{
			//减少CPU消耗。
			XThread::Sleep(1);
		}

		//心跳检测。
		ret = CheckClient();

		//检测客户端。
		ret = DoSelect();
		if (ret < 0)
			pThread->Exit();

		//接收数据。
		RecvData();

		//发送数据。
		SendData();

		//处理数据。
		DoData();
	}

	XInfo("    XReceiveServer<ID=%d>:OnRun() End\n", _ID);
}

void XServer::UpdateFrameTimeDelta()
{
	//循环计时。
	_CurFrameTime = XTimer::GetTimeByMicroseconds();
	_FrameTimeDelta = _CurFrameTime - _LastFrameTime;
	_LastFrameTime = _CurFrameTime;
}

int XServer::CheckClient()
{
	//是否有新客户端加入？
	if (!_AllClientsCache.empty())
	{
		std::lock_guard<std::mutex> lock(_AllClientsCacheMutex);
		for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClientsCache.begin(); iter != _AllClientsCache.end(); ++iter)
		{
			_AllClients.insert(std::pair<SOCKET, std::shared_ptr<XClient>>(iter->first, iter->second));

			//不在这里回掉，使用在accept客户端并添加到缓冲区列表时调用。
			//if (_pGlobalEventObj)
			//	_pGlobalEventObj->OnClientJoin(iter->second);
		}
		_AllClientsCache.clear();

		_ClientChange = true;
	}

	//是否有客户端连接？
	if (_AllClients.empty())
	{
		return -1;
	}

	return 0;
}

int XServer::CheckHeart()
{
	int ret = 0;
	//心跳检测。
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); )
	{
		if (iter->second->CheckHeartTime(_FrameTimeDelta))
		{
			if (_pGlobalEventObj)
				_pGlobalEventObj->OnClientLeave(iter->second);

			iter = _AllClients.erase(iter);
			_ClientChange = true;

			++ret;
			continue;
		}

		++iter;
	}

	return ret;
}

int XServer::DoSelect()
{
	//设置_FdRead
	if (_ClientChange)
	{
		FD_ZERO(&_FdRead);

		_MaxSocketID = 0;
		for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); ++iter)
		{
			FD_SET(iter->first, &_FdRead);
			if (_MaxSocketID < iter->first)
				_MaxSocketID = iter->first;
		}

		memcpy(&_FdSetCache, &_FdRead, sizeof(fd_set));

		_ClientChange = false;
	}
	else
	{
		memcpy(&_FdRead, &_FdSetCache, sizeof(fd_set));
	}

	//设置_FdWrite
	bool bHasCanWriteClient = false;
	FD_ZERO(&_FdWrite);
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); ++iter)
	{
		if (iter->second->HasData())
		{
			FD_SET(iter->first, &_FdWrite);
			bHasCanWriteClient = true;
		}
	}

	//设置1毫秒间隔，可以提高客户端连接select效率。
	//使用时间间隔可以提高客户端连接速度。使用阻塞模式更快。但此处不能使用组塞模式，需要执行定时检测任务。
	timeval tv = { 0, 0 };

	int ret;
	if (bHasCanWriteClient)
		ret = select((int)_MaxSocketID + 1, &_FdRead, &_FdWrite, nullptr, &tv);
	else
		ret = select((int)_MaxSocketID + 1, &_FdRead, nullptr, nullptr, &tv);

	if (SOCKET_ERROR == ret)
	{
		XError("Select!\n");
		return -1;
	}

	return 0;
}

void XServer::RecvData()
{
	//从客户端接收数据
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
	{
		if (FD_ISSET(iter->first, &_FdRead))
		{
			int ret = iter->second->RecvData();
			if (ret != 0)			//不等于0，ret为1说明接收缓冲区满了，主动断开连接。
			{
				if (_pGlobalEventObj)
					_pGlobalEventObj->OnClientLeave(iter->second);

				iter = _AllClients.erase(iter);
				_ClientChange = true;
				continue;
			}
		}

		++iter;
	}
}

void XServer::SendData()
{	
	//向客户端发送数据
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
	{
		if (FD_ISSET(iter->first, &_FdWrite))
		{
			int ret = iter->second->SendData();
			if (ret < 0)			//小于0，ret为1时说明发送缓冲区为空，不应该断开。
			{
				if (_pGlobalEventObj)
					_pGlobalEventObj->OnClientLeave(iter->second);

				iter = _AllClients.erase(iter);
				_ClientChange = true;
				continue;
			}
		}

		++iter;
	}
}

void XServer::DoData()
{
	//处理客户端接收的数据
	for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); ++iter)
	{
		iter->second->DoData();
	}
}

