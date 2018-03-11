#include "XServer.h"


_Client::_Client(SOCKET client)
{
	_Socket = client;
	memset(_RecvBuffer, 0, _RECV_BUFFER_SIZE_);
	_RecvStartPos = 0;
	memset(_SendBuffer, 0, _SEND_BUFFER_SIZE_);
	_SendStartPos = 0;
}

_Client::~_Client()
{

}

void _Client::Init(IEvent* pEventObj, _ReceiveServer* pReceiveServerObj)
{
	_pNetEventObj = pEventObj;
	_pReceiveServerObj = pReceiveServerObj;
}

int _Client::RecvData()
{
	//接收数据到接收缓冲区中
	char* pBuffer = _RecvBuffer + _RecvStartPos;
	int size = recv(_Socket, pBuffer, _RECV_BUFFER_SIZE_ - _RecvStartPos, 0);
	if (_pNetEventObj)
		_pNetEventObj->OnNetRecv(this);
	if (SOCKET_ERROR == size)
	{
		printf("OK:Client<Socket=%d> off!\n", (int)_Socket);
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:Client<Socket=%d> quit!\n", (int)_Socket);
		return -2;
	}

	_RecvStartPos += size;

	//数据缓冲区长度大于消息头长度
	while (_RecvStartPos >= sizeof(MsgHeader))
	{
		MsgHeader* pHeader = (MsgHeader*)_RecvBuffer;
		//数据缓冲区长度大于消息长度
		if (_RecvStartPos >= pHeader->_MsgLength)
		{
			//数据缓冲区剩余未处理数据长度
			int len = _RecvStartPos - pHeader->_MsgLength;

			if (_pNetEventObj)
				_pNetEventObj->OnNetMsg(this, pHeader, _pReceiveServerObj);

			//数据缓冲区剩余未处理数据前移 -- 此处为模拟处理
			memcpy(_RecvBuffer, _RecvBuffer + pHeader->_MsgLength, len);
			_RecvStartPos = len;
		}
		else
		{
			//数据缓冲区剩余未处理数据不够一条完整消息
			break;
		}
	}

	return 0;
}

int _Client::SendData(MsgHeader* pHeader)
{
	const char* pBuffer = (const char*)pHeader;
	int nSendBufferSize = pHeader->_MsgLength;

	while (true)
	{
		if (_SendStartPos + nSendBufferSize >= _SEND_BUFFER_SIZE_)
		{
			int len = _SEND_BUFFER_SIZE_ - _SendStartPos;
			memcpy(_SendBuffer + _SendStartPos, pBuffer, len);
			pBuffer += len;
			nSendBufferSize -= len;

			int ret = send(_Socket, _SendBuffer, _SEND_BUFFER_SIZE_, 0);
			if (_pNetEventObj)
				_pNetEventObj->OnNetSend(this);

			_SendStartPos = 0;

			if (SOCKET_ERROR == ret)
			{
				return -1;
			}
		}
		else
		{
			memcpy(_SendBuffer + _SendStartPos, pBuffer, nSendBufferSize);
			_SendStartPos += nSendBufferSize;
			break;
		}
	}

	return 0;
}

XSendTask::XSendTask(_Client* pClient, MsgHeader* pHeader)
{
	_pClient = pClient;
	_pHeader = pHeader;
}

XSendTask::~XSendTask()
{
	delete _pHeader;
}

void XSendTask::DoTask()
{
	_pClient->SendData(_pHeader);
}

_ReceiveServer::_ReceiveServer()
{
	_pNetEventObj = 0;
	_ClientChange = true;
}

_ReceiveServer::~_ReceiveServer()
{

}

void _ReceiveServer::SetNetEventObj(IEvent* pEventObj)
{
	_pNetEventObj = pEventObj;
}

int _ReceiveServer::Start()
{
	std::thread t(std::mem_fn(&_ReceiveServer::OnRun), this);
	t.detach();

	_TaskServer.Start();

	return 0;
}

int _ReceiveServer::OnRun()
{
	while (true)
	{
		if (!_AllClientsCache.empty())
		{
			std::lock_guard<std::mutex> lock(_AllClientsCacheMutex);
			for (std::map<SOCKET, _Client*>::iterator iter = _AllClientsCache.begin(); iter != _AllClientsCache.end(); ++iter)
			{
				_AllClients.insert(std::pair<SOCKET, _Client*>(iter->first, iter->second));
			}
			_AllClientsCache.clear();

			_ClientChange = true;
		}

		if (_AllClients.empty())
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			continue;
		}

		fd_set fdRead;
		FD_ZERO(&fdRead);

		if (_ClientChange)
		{
			_ClientChange = false;

			_MaxSocketID = 0;
			for (std::map<SOCKET, _Client*>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); ++iter)
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
		timeval tv = { 0, 10 };			//使用时间间隔可以提高客户端连接速度。使用阻塞模式更快。
		//timeval tv = { 0, 0 };			//客户端连接速度变慢。
		int ret = select((int)_MaxSocketID + 1, &fdRead, NULL, NULL, NULL);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select!\n");
			return -1;
		}
		else if (0 == ret)
		{
			continue;
		}

#ifdef _WIN32
		for (unsigned int i = 0; i < fdRead.fd_count; ++i)
		{
			std::map<SOCKET, _Client*>::iterator iter = _AllClients.find(fdRead.fd_array[i]);
			if (iter != _AllClients.end())
			{
				//int ret = RecvData(iter->second);
				int ret = iter->second->RecvData();
				if (ret < 0)
				{
					if (_pNetEventObj)
						_pNetEventObj->OnClientLeave(iter->second);

					iter = _AllClients.erase(iter);
					_ClientChange = true;
					continue;
				}
			}
		}
#else
		for (std::map<SOCKET, _Client*>::iterator iter = _AllClients.begin(); iter != _AllClients.end();)
		{
			if (FD_ISSET(iter->first, &fdRead))
			{
				int ret = RecvData(iter->second);
				if (ret < 0)
				{
					if (_pNetEventObj)
						_pNetEventObj->OnClientLeave(iter->second);

					iter = _AllClients.erase(iter);
					_ClientChange = true;
					continue;
				}
			}

			++iter;
		}
#endif

	}
	return 0;
}

void _ReceiveServer::AddClient(_Client* pClient)
{
	std::lock_guard<std::mutex> lock(_AllClientsCacheMutex);
	_AllClientsCache.insert(std::pair<SOCKET, _Client*>(pClient->GetSocket(), pClient));
}

int _ReceiveServer::GetClientNum()
{
	return (int)_AllClients.size() + (int)_AllClientsCache.size();
}

void _ReceiveServer::AddTask(XTask* pTask)
{
	_TaskServer.AddTask(pTask);
}

//--------------------------------------------------------------------------------------------------------------------

_ListenServer::_ListenServer()
{
	_Socket = INVALID_SOCKET;
}

_ListenServer::~_ListenServer()
{

}

int _ListenServer::Init()
{
	//初始化网络环境
#ifdef _WIN32
	WORD wsaVersion = MAKEWORD(2, 2);
	WSADATA wsaData = {};
	int iError = WSAStartup(wsaVersion, &wsaData);
	if (iError)
	{
		printf("Error:WSAStartup!\n");
		return -1;
	}
	else
	{
		printf("OK:WSAStartup!\n");
	}
#endif

	return 0;
}

int _ListenServer::Done()
{
	//销毁网络环境
#ifdef _WIN32
	int iError = WSACleanup();
	if (SOCKET_ERROR == iError)
	{
		printf("Error:WSACleanup!\n");
		return -1;
	}
	else
	{
		printf("OK:WSACleanup!\n");
	}
#endif

	return 0;
}

int _ListenServer::Open()
{
	_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _Socket)
	{
		printf("Error:socket!\n");
		return -1;
	}
	else
	{
		printf("OK<Socket=%d>:socket!\n", (int)_Socket);
	}

	return 0;
}

int _ListenServer::Bind(const char* ip, unsigned short port)
{
	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
#ifdef _WIN32
	if (ip)
	{
		sin.sin_addr.S_un.S_addr = inet_addr(ip);
	}
	else
	{
		sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	}
#else
	if (ip)
	{
		sin.sin_addr.s_addr = inet_addr(ip);
	}
	else
	{
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
#endif
	sin.sin_port = htons(port);
	if (SOCKET_ERROR == bind(_Socket, (sockaddr*)&sin, sizeof(sockaddr_in)))
	{
		printf("Error<Socket=%d>:bind!\n", (int)_Socket);
		return -1;
	}
	else
	{
		printf("OK<Socket=%d>:bind!\n", (int)_Socket);
	}

	return 0;
}

int _ListenServer::Listen(int n)
{
	if (SOCKET_ERROR == listen(_Socket, n))
	{
		printf("Error<Socket=%d>:listen!\n", (int)_Socket);
		return -1;
	}
	else
	{
		printf("OK<Socket=%d>:listen!\n", (int)_Socket);
	}

	return 0;
}

int _ListenServer::Accept()
{
	sockaddr_in sinClient = {};
	int sinLen = sizeof(sockaddr_in);
	SOCKET client = INVALID_SOCKET;
#ifdef _WIN32
	client = accept(_Socket, (sockaddr*)&sinClient, &sinLen);
#else
	client = accept(_Socket, (sockaddr*)&sinClient, (socklen_t*)&sinLen);
#endif
	if (client == INVALID_SOCKET)
	{
		printf("Error<Socket=%d>:accept!\n", (int)_Socket);
		return -1;
	}
	else
	{
		_ReceiveServer* pLessServer = _AllServers[0];
		for (auto pServer : _AllServers)
		{
			if (pLessServer->GetClientNum() > pServer->GetClientNum())
			{
				pLessServer = pServer;
			}
		}

		_Client* pClient = new _Client(client);
		pClient->Init(this, pLessServer);
		pLessServer->AddClient(pClient);

		OnClientJoin(pClient);
	}
	
	return 0;
}

int _ListenServer::Start()
{
	for (int i = 0; i < _SERVER_SIZE_; ++i)
	{
		_ReceiveServer* pServer = new _ReceiveServer();
		pServer->SetNetEventObj(this);
		pServer->Start();
		_AllServers.push_back(pServer);
	}

	return 0;
}

int _ListenServer::Close()
{
	if (INVALID_SOCKET != _Socket)
	{
#ifdef _WIN32
		if (SOCKET_ERROR == closesocket(_Socket))
		{
			printf("Error<Socket=%d>:closesocket!\n", (int)_Socket);
		}
		else
		{
			printf("OK<Socket=%d>:closesocket!\n", (int)_Socket);
		}
#else
		if (SOCKET_ERROR == close(_Socket))
		{
			printf("Error<Socket=%d>:close!\n", (int)_Socket);
		}
		else
		{
			printf("OK<Socket=%d>:close!\n", (int)_Socket);
		}
#endif
		_Socket = INVALID_SOCKET;
	}

	return 0;
}

int _ListenServer::IsRun()
{
	return _Socket == INVALID_SOCKET ? 0 : 1;
}

int _ListenServer::OnRun()
{
	while (IsRun())
	{
		OnRunBegin();

		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_Socket, &fdRead);

		//设置10毫秒间隔，可以提高数据接受和发送select效率。
		timeval tv = { 0, 10 };			
		int ret = select((int)_Socket + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error<Socket=%d>:Select!\n", (int)_Socket);
			return -1;
		}
		else if (0 == ret)
		{
			continue;
		}

		if (FD_ISSET(_Socket, &fdRead))
		{
			FD_CLR(_Socket, &fdRead);
			Accept();
		}
	}
	return 0;
}

