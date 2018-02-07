#include "Server.h"

Server::Server()
{
	_Socket = NULL;
	memset(_Buffer, 0, sizeof(_Buffer));
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
	SOCKET _client = INVALID_SOCKET;
#ifdef _WIN32
	_client = accept(_Socket, (sockaddr*)&sinClient, &sinLen);
#else
	_client = accept(_Socket, (sockaddr*)&sinClient, (socklen_t*)&sinLen);
#endif
	if (_client == INVALID_SOCKET)
	{
		printf("Error<Socket=%d>:accept!\n", (int)_Socket);
		return -1;
	}
	else
	{
		printf("OK<Socket=%d>:accept Client<Socket=%d>!\n", (int)_Socket, (int)_client);

		MsgNewUser msgNewUser;
		msgNewUser.user = (int)_client;
		SendDataToAll(&msgNewUser);

		_AllClients.push_back(_client);
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

int Server::CloseOne(SOCKET client)
{
#ifdef _WIN32
	if (SOCKET_ERROR == closesocket(client))
	{
		printf("Error<Socket=%d>:closesocket Client<Socket=%d>!\n", (int)_Socket, (int)client);
	}
	else
	{
		printf("OK<Socket=%d>:closesocket Client<Socket=%d>!\n", (int)_Socket, (int)client);
	}
#else
	if (SOCKET_ERROR == close(client))
	{
		printf("Error:<Socket=%d>close Client<Socket=%d>!\n", (int)_Socket, (int)client);
	}
	else
	{
		printf("OK:<Socket=%d>close Client<Socket=%d>!\n", (int)_Socket, (int)client);
	}
#endif

	return 0;
}

int Server::CloseAll()
{
#ifdef _WIN32
	for (std::vector<SOCKET>::iterator it = _AllClients.begin(); it != _AllClients.end(); ++it)
	{
		if (SOCKET_ERROR == closesocket(*it))
		{
			printf("Error<Socket=%d>:closesocket Client<Socket=%d>!\n", (int)_Socket, (int)*it);
		}
		else
		{
			printf("OK<Socket=%d>:closesocket Client<Socket=%d>!\n", (int)_Socket, (int)*it);
		}
	}
	_AllClients.clear();
#else
	for (std::vector<SOCKET>::iterator it = _AllClients.begin(); it != _AllClients.end(); ++it)
	{
		if (SOCKET_ERROR == close(*it))
		{
			printf("Error:<Socket=%d>close Client<Socket=%d>!\n", (int)_Socket, (int)*it);
		}
		else
		{
			printf("OK:<Socket=%d>close Client<Socket=%d>!\n", (int)_Socket, (int)*it);
		}
	}
	_AllClients.clear();
#endif
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
			FD_SET(_AllClients[i], &fdRead);
			if (MaxSocket < _AllClients[i])
				MaxSocket = _AllClients[i];
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

		for (std::vector<SOCKET>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); )
		{
			if (FD_ISSET(*iter, &fdRead))
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

int Server::RecvData(SOCKET client)
{
	int size = recv(client, _Buffer, sizeof(MsgHeader), 0);
	if (SOCKET_ERROR == size)
	{
		printf("OK<Socket=%d>:Client<Socket=%d> off!\n", (int)_Socket, (int)client);
		return -1;
	}
	else if (size == 0)
	{
		printf("OK<Socket=%d>:Client<Socket=%d> quit!\n", (int)_Socket, (int)client);
		return -2;
	}

	MsgHeader* request = (MsgHeader*)_Buffer;
	printf("----<Socket=%d> From Client<Socket=%d> = Msg Type:%d - Length:%d\n", (int)_Socket, (int)client, request->msgType, request->msgLength);
	recv(client, _Buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);

	switch (request->msgType)
	{
	case MSG_LOGIN:
	{
		MsgLogin* login = (MsgLogin*)request;
		printf("----<Socket=%d> From Client<Socket=%d> = MSG_LOGIN : Name:%s - Pwd:%s\n", (int)_Socket, (int)client, login->name, login->pwd);

		MsgLoginRes respond;
		SendData(client, &respond);
	}
	break;
	case MSG_LOGOUT:
	{
		MsgLogout* logout = (MsgLogout*)request;
		printf("----<Socket=%d> From Client<Socket=%d> = MSG_LOGOUT : Name:%s\n", (int)_Socket, (int)client, logout->name);

		MsgLogoutRes respond;
		SendData(client, &respond);
	}
	break;
	default:
	{

	}
	}

	return 0;
}

int Server::SendData(SOCKET client, MsgHeader* pHeader)
{
	if (IsRun() && pHeader)
		return send(client, (const char*)pHeader, pHeader->msgLength, 0);

	return -1;
}

int Server::SendDataToAll(MsgHeader* pHeader)
{
	for (std::vector<SOCKET>::iterator iter = _AllClients.begin(); iter != _AllClients.end(); ++iter)
	{
		SendData(*iter, pHeader);
	}

	return 0;
}