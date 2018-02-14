#include "Server.h"

Server::Server()
{
	_Socket = INVALID_SOCKET;
	memset(_RecvBuffer, 0, sizeof(_RecvBuffer));
	_PackageNum = 0;
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
		printf("OK<Socket=%d>:accept Client<Socket=%d>!\n", (int)_Socket, (int)client);

		MsgNewUser msgNewUser;
		msgNewUser._UserID = (int)client;
		SendDataToAll(&msgNewUser);

		_AllClients.push_back(new ClientInfo(client));
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
	if (IsRun())
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_Socket, &fdRead);

		SOCKET MaxSocket = _Socket;
		for (size_t i = 0; i < _AllClients.size(); ++i)
		{
			FD_SET(_AllClients[i]->GetSocket(), &fdRead);
			if (MaxSocket < _AllClients[i]->GetSocket())
				MaxSocket = _AllClients[i]->GetSocket();
		}

		timeval tv = { 0, 0 };
		int ret = select((int)MaxSocket + 1, &fdRead, NULL, NULL, &tv);
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

		for (std::vector<ClientInfo*>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); )
		{
			if (FD_ISSET((*iter)->GetSocket(), &fdRead))
			{
				int ret = RecvData(*iter);
				if (ret < 0)
				{
					CloseOne(*iter);
					iter = _AllClients.erase(iter);
					continue;
				}
			}

			++iter;
		}
	}
	return 0;
}

int Server::SendData(ClientInfo* pClientInfo, MsgHeader* pHeader)
{
	if (IsRun() && pClientInfo && pHeader)
		return send(pClientInfo->GetSocket(), (const char*)pHeader, pHeader->_MsgLength, 0);

	return -1;
}

int Server::SendDataToAll(MsgHeader* pHeader)
{
	for (std::vector<ClientInfo*>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); ++iter)
	{
		SendData(*iter, pHeader);
	}

	return 0;
}

//unsigned long n = 0;
int Server::RecvData(ClientInfo* pClientInfo)
{
	//接收数据到接收缓冲区中
	int size = recv(pClientInfo->GetSocket(), _RecvBuffer, _BUFFER_SIZE_, 0);
	if (SOCKET_ERROR == size)
	{
		printf("OK<Socket=%d>:Client<Socket=%d> off!\n", (int)_Socket, (int)pClientInfo->GetSocket());
		return -1;
	}
	else if (size == 0)
	{
		printf("OK<Socket=%d>:Client<Socket=%d> quit!\n", (int)_Socket, (int)pClientInfo->GetSocket());
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

int Server::OnNetMsg(ClientInfo* pClientInfo, MsgHeader* pHeader)
{
	_PackageNum++;
	if (_Timer.GetTime() > 1.0)
	{
		printf("---------接收到数据包：%d 个。\n", _PackageNum);

		_PackageNum = 0;
		_Timer.UpdateTime();
	}


	switch (pHeader->_MsgType)
	{
	case MSG_LOGIN:
	{
		MsgLogin* login = (MsgLogin*)pHeader;
		//printf("----<Socket=%d> From Client<Socket=%d> = MSG_LOGIN : Name:%s - Pwd:%s\n", (int)_Socket, (int)pClientInfo->GetSocket(), login->_Name, login->_Pwd);

		MsgLoginRes respond;
		//SendData(pClientInfo, &respond);
	}
	break;
	case MSG_LOGOUT:
	{
		MsgLogout* logout = (MsgLogout*)pHeader;
		//printf("----<Socket=%d> From Client<Socket=%d> = MSG_LOGOUT : Name:%s\n", (int)_Socket, (int)pClientInfo->GetSocket(), logout->_Name);

		MsgLogoutRes respond;
		//SendData(pClientInfo, &respond);
	}
	break;
	default:
	{
		//printf("----<Socket=%d> From Client<Socket=%d> = MSG_Error\n", (int)_Socket, (int)pClientInfo->GetSocket());

		MsgError respond;
		//SendData(pClientInfo, &respond);
	}
	}

	return 0;
}