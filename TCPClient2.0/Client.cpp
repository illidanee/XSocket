#include "Client.h"

Client::Client()
{
	_Socket = INVALID_SOCKET;
	memset(_Buffer, 0, sizeof(_Buffer));
}

Client::~Client()
{

}

int Client::Init()
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
#endif // _WIN32

	return 0;
}

int Client::Done()
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
#endif // _WIN32

	return 0;
}

int Client::Connect(const char* ip, unsigned short port)
{
	_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _Socket)
	{
		printf("Error:socket!\n");
		return -1;
	}
	else
	{
		printf("OK:socket!\n");
	}

	sockaddr_in sinServer = {};
	sinServer.sin_family = AF_INET;
#ifdef _WIN32
	sinServer.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	sinServer.sin_addr.s_addr = inet_addr(ip);
#endif // _WIN32
	sinServer.sin_port = htons(port);
	int sinLen = sizeof(sockaddr_in);
	if (SOCKET_ERROR == connect(_Socket, (sockaddr*)&sinServer, sinLen))
	{
		printf("Error:connect!\n");
		return -2;
	}
	else
	{
		printf("OK:connect!\n");
	}

	return 0;
}

int Client::Close()
{
	if (SOCKET_ERROR != _Socket)
	{
#ifdef _WIN32
		if (SOCKET_ERROR == closesocket(_Socket))
		{
			printf("Error:closesocket!\n");
			return -1;
		}
		else
		{
			printf("OK:closesocket!\n");
		}
#else
		if (SOCKET_ERROR == close(_Socket))
		{
			printf("Error:close!\n");
			return -1;
		}
		else
		{
			printf("OK:close!\n");
		}
#endif // _WIN32

		_Socket = SOCKET_ERROR;
	}
	
	return 0;
}

int Client::IsRun()
{
	return _Socket == SOCKET_ERROR ? 0 : 1;
}

int Client::OnRun()
{
	if (IsRun())
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_Socket, &fdRead);

		timeval tv = { 0, 0 };
		int ret = select((int)_Socket + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:select!\n");
			Close();
			return -1;
		}

		if (FD_ISSET(_Socket, &fdRead))
		{
			FD_CLR(_Socket, &fdRead);

			int ret = RecvData();
			if (ret < 0)
			{
				Close();
				return -2;
			}
		}
	}
	return 0;
}

int Client::SendData(MsgHeader* pHeader)
{
	if (IsRun() && pHeader)
		return send(_Socket, (const char*)pHeader, pHeader->msgLength, 0);

	return -1;
}

int Client::RecvData()
{
	int size = recv(_Socket, _Buffer, 409600000, 0);
	printf("--Recv Len = %d \n", size);
	return 0;

	if (SOCKET_ERROR == size)
	{
		printf("OK:Server off!\n");
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:Server quit!\n");
		return -2;
	}

	MsgHeader* request = (MsgHeader*)_Buffer;
	printf("----Recieve Msg Type:%d - Length:%d\n", request->msgType, request->msgLength);
	recv(_Socket, _Buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);

	switch (request->msgType)
	{
	case MSG_LOGIN_RES:
	{
		MsgLoginRes* login = (MsgLoginRes*)request;
		printf("----Login Ret:%d\n", login->res);
	}
	break;
	case MSG_LOGOUT_RES:
	{
		MsgLogoutRes* logout = (MsgLogoutRes*)request;
		printf("----Logout Ret:%d\n", logout->res);
	}
	break;
	case MSG_NEWUSER:
	{
		MsgNewUser* newUser = (MsgNewUser*)request;
		printf("----New User <Socket=%d> Join Server!\n", newUser->user);
	}
	break;
	default:
	{

	}
	}

	return 0;
}