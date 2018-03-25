#include "XReceiveServer.h"

XReceiveServer::XReceiveServer()
{
	_pNetEventObj = nullptr;
	_LastTime = 0;
	_ClientChange = true;
}

XReceiveServer::~XReceiveServer()
{

}

void XReceiveServer::SetNetEventObj(XIEvent* pEventObj)
{
	_pNetEventObj = pEventObj;
}

int XReceiveServer::Start()
{
	std::thread t(std::mem_fn(&XReceiveServer::OnRun), this);
	t.detach();

	_TaskServer.Start();

	return 0;
}

int XReceiveServer::OnRun()
{
	//初始化计时。
	_LastTime = XTimer::GetTimeByMicroseconds();

	while (true)
	{
		//心跳检测！
		time_t curTime = XTimer::GetTimeByMicroseconds();
		time_t delta = curTime - _LastTime;
		_LastTime = curTime;

		for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); )
		{
			if (iter->second->CheckHeartTime(delta))
			{
				if (_pNetEventObj)
					_pNetEventObj->OnClientLeave(iter->second.get());

#ifdef _WIN32
				closesocket(iter->first);
#else
				close(iter->first);
#endif
				iter = _AllClients.erase(iter);
				_ClientChange = true;
				continue;
			}
			iter->second->CheckSendTime(delta);
			++iter;
		}

		//是否有新客户端加入？
		if (!_AllClientsCache.empty())
		{
			std::lock_guard<std::mutex> lock(_AllClientsCacheMutex);
			for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClientsCache.begin(); iter != _AllClientsCache.end(); ++iter)
			{
				_AllClients.insert(std::pair<SOCKET, std::shared_ptr<XClient>>(iter->first, iter->second));
			}
			_AllClientsCache.clear();

			_ClientChange = true;
		}

		//是否有客户端连接？
		if (_AllClients.empty())
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			continue;
		}

		//检查是否有客户端向服务器发送数据。
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

		//设置10毫秒间隔，可以提高客户端连接select效率。
		timeval tv = { 0, 1000 };			//使用时间间隔可以提高客户端连接速度。使用阻塞模式更快。
											//timeval tv = { 0, 0 };			//客户端连接速度变慢。
		int ret = select((int)_MaxSocketID + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select!\n");
			return -1;
		}
		else if (0 == ret)
		{
			continue;
		}

		//接收客户端数据

		//#ifdef _WIN32
		//		for (unsigned int i = 0; i < fdRead.fd_count; ++i)
		//		{
		//			std::map<SOCKET, std::shared_ptr<_Client>>::iterator iter = _AllClients.find(fdRead.fd_array[i]);
		//			if (iter != _AllClients.end())
		//			{
		//				int ret = iter->second->RecvData();
		//				if (ret < 0)
		//				{
		//					if (_pNetEventObj)
		//						_pNetEventObj->OnClientLeave(iter->second.get());
		//
		//					iter = _AllClients.erase(iter);
		//					_ClientChange = true;
		//					continue;
		//				}
		//			}
		//		}
		//#else
		for (std::map<SOCKET, std::shared_ptr<XClient>>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
		{
			if (FD_ISSET(iter->first, &fdRead))
			{
				int ret = iter->second->RecvData();
				if (ret < 0)
				{
					if (_pNetEventObj)
						_pNetEventObj->OnClientLeave(iter->second.get());

					closesocket(iter->first);
					iter = _AllClients.erase(iter);
					_ClientChange = true;
					continue;
				}
			}

			++iter;
		}
		//#endif

	}
	return 0;
}

void XReceiveServer::AddClient(const std::shared_ptr<XClient>& pClient)
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