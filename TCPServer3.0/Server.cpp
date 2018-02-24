#include "Server.h"

MicroServer::MicroServer()
{
	_PackageNum = 0;
	memset(_RecvBuffer, 0, sizeof(_RecvBuffer));
}

MicroServer::~MicroServer()
{

}

int MicroServer::Start()
{
	std::thread t(std::mem_fun(&MicroServer::OnRun), this);
	t.detach();

	return 0;
}

int MicroServer::OnRun()
{
	while (true)
	{
		if (!_AllClientsCache.empty())
		{
			std::lock_guard<std::mutex> lock(_Mutex);
			for (auto pClient : _AllClientsCache)
			{
				_AllClients.push_back(pClient);
			}
			_AllClientsCache.clear();
		}

		if (_AllClients.empty())
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1000));
			continue;
		}

		fd_set fdRead;
		FD_ZERO(&fdRead);

		SOCKET MaxSocketID = _AllClients[0]->GetSocket();
		for (auto pClient : _AllClients)
		{
			FD_SET(pClient->GetSocket(), &fdRead);
			if (MaxSocketID < pClient->GetSocket())
				MaxSocketID = pClient->GetSocket();
		}

		timeval tv = { 0, 1 };
		int ret = select((int)MaxSocketID + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select!\n");
			return -1;
		}

		for (auto pClient : _AllClients)
		{
			if (FD_ISSET(pClient->GetSocket(), &fdRead))
			{
				int ret = RecvData(pClient);
				//暂不处理客户端断开连接
			}
		}
	}
	return 0;
}

int MicroServer::SendData(ClientInfo* pClientInfo, MsgHeader* pHeader)
{
	if (pClientInfo && pHeader)
		return send(pClientInfo->GetSocket(), (const char*)pHeader, pHeader->_MsgLength, 0);

	return -1;
}

int MicroServer::SendDataToAll(MsgHeader* pHeader)
{
	for (auto pClient : _AllClients)
	{
		SendData(pClient, pHeader);
	}

	return 0;
}

int MicroServer::RecvData(ClientInfo* pClientInfo)
{
	//接收数据到接收缓冲区中
	int size = recv(pClientInfo->GetSocket(), _RecvBuffer, _BUFFER_SIZE_, 0);
	if (SOCKET_ERROR == size)
	{
		printf("OK:Client<Socket=%d> off!\n", (int)pClientInfo->GetSocket());
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:Client<Socket=%d> quit!\n", (int)pClientInfo->GetSocket());
		return -2;
	}

	//将接收缓冲区数据拷贝到数据缓冲区
	memcpy(pClientInfo->GetDataBuffer() + pClientInfo->GetStartPos(), _RecvBuffer, size);
	pClientInfo->SetStartPos(pClientInfo->GetStartPos() + size);

	//数据缓冲区长度大于消息头长度
	while (pClientInfo->GetStartPos() >= sizeof(MsgHeader))
	{
		MsgHeader* pHeader = (MsgHeader*)pClientInfo->GetDataBuffer();
		//数据缓冲区长度大于消息长度
		if (pClientInfo->GetStartPos() >= pHeader->_MsgLength)
		{
			//数据缓冲区剩余未处理数据长度
			int len = pClientInfo->GetStartPos() - pHeader->_MsgLength;
			//printf("----%d - <Socket=%d> From Client<Socket=%d> = Msg Type:%d - Length:%d\n", n++, (int)_Socket, (int)pClientInfo->GetSocket(), pHeader->_MsgType, pHeader->_MsgLength);
			OnNetMsg(pClientInfo, pHeader);

			//数据缓冲区剩余未处理数据前移 -- 此处为模拟处理
			memcpy(pClientInfo->GetDataBuffer(), pClientInfo->GetDataBuffer() + pHeader->_MsgLength, len);
			pClientInfo->SetStartPos(len);
		}
		else
		{
			//数据缓冲区剩余未处理数据不够一条完整消息
			break;
		}
	}

	return 0;
}

int MicroServer::OnNetMsg(ClientInfo* pClientInfo, MsgHeader* pHeader)
{
	_PackageNum++;

	return 0;
}

void MicroServer::AddClient(ClientInfo* pClient)
{
	std::lock_guard<std::mutex> lock(_Mutex);
	_AllClientsCache.push_back(pClient);
}

int MicroServer::GetClientNum()
{
	return _AllClients.size() + _AllClientsCache.size();
}

//--------------------------------------------------------------------------------------------------------------------

Server::Server()
{
	_Socket = INVALID_SOCKET;
}

Server::~Server()
{

}

int Server::Init()
{
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

	_Timer.XInit();

	return 0;
}

int Server::Done()
{
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

	_Timer.XDone();

	return 0;
}

int Server::Open()
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

int Server::Bind(const char* ip, unsigned short port)
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

int Server::Listen(int n)
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

int Server::Accept()
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
		//printf("OK<Socket=%d>:accept Client<Socket=%d>!\n", (int)_Socket, (int)client);

		//MsgNewUser msgNewUser;
		//msgNewUser._UserID = (int)client;
		//SendDataToAll(&msgNewUser);

		ClientInfo* pNewClientInfo = new ClientInfo(client);

		_AllClients.push_back(pNewClientInfo);

		MicroServer* pLessServer = _AllServers[0];
		for (auto pServer : _AllServers)
		{
			if (pLessServer->GetClientNum() > pServer->GetClientNum())
			{
				pLessServer = pServer;
			}
		}
		pLessServer->AddClient(pNewClientInfo);
	}
	
	return 0;
}

int Server::Start()
{
	for (int i = 0; i < _SERVER_SIZE_; ++i)
	{
		MicroServer* pServer = new MicroServer();
		pServer->Start();
		_AllServers.push_back(pServer);
	}

	return 0;
}

int Server::Close()
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

int Server::CloseOne(ClientInfo* pClientInfo)
{
	if (pClientInfo)
	{
#ifdef _WIN32
		if (SOCKET_ERROR == closesocket(pClientInfo->GetSocket()))
		{
			printf("Error<Socket=%d>:closesocket Client<Socket=%d>!\n", (int)_Socket, (int)pClientInfo->GetSocket());
		}
		else
		{
			printf("OK<Socket=%d>:closesocket Client<Socket=%d>!\n", (int)_Socket, (int)pClientInfo->GetSocket());
		}
#else
		if (SOCKET_ERROR == close(pClientInfo->GetSocket()))
		{
			printf("Error:<Socket=%d>close Client<Socket=%d>!\n", (int)_Socket, (int)pClientInfo->GetSocket());
		}
		else
		{
			printf("OK:<Socket=%d>close Client<Socket=%d>!\n", (int)_Socket, (int)pClientInfo->GetSocket());
		}
#endif
		delete pClientInfo;
		pClientInfo = NULL;
	}

	return 0;
}

int Server::CloseAll()
{
	for (std::vector<ClientInfo*>::iterator it = _AllClients.begin(); it != _AllClients.end(); ++it)
	{
		CloseOne(*it);
	}
	_AllClients.clear();

	return 0;
}

int Server::IsRun()
{
	return _Socket == INVALID_SOCKET ? 0 : 1;
}

int Server::OnRun()
{
	while (IsRun())
	{
		if (_Timer.GetTime() > 1.0)
		{
			int num = 0;
			for (auto pServer : _AllServers)
				num += pServer->_PackageNum;

			printf("--当前客户端连接数:%4d  --接收到数据包:%d 个。\n", (int)_AllClients.size(), num);

			for (auto pServer : _AllServers)
				pServer->_PackageNum = 0;

			_Timer.UpdateTime();
		}

		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_Socket, &fdRead);

		timeval tv = { 0, 1 };
		int ret = select((int)_Socket + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error<Socket=%d>:Select!\n", (int)_Socket);
			return -1;
		}

		if (FD_ISSET(_Socket, &fdRead))
		{
			FD_CLR(_Socket, &fdRead);
			Accept();
		}
	}
	return 0;
}