#include "Client.h"

Client::Client()
{
	_socket = INVALID_SOCKET;
	memset(_buffer, 0, sizeof(_buffer));
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
	Close();

	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _socket)
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
	sinServer.sin_addr.S_un.S_addr = inet_addr("192.168.0.90");
#else
	sinServer.sin_addr.s_addr = inet_addr("192.168.0.90");
#endif // _WIN32
	sinServer.sin_port = htons(port);
	int sinLen = sizeof(sockaddr_in);
	if (SOCKET_ERROR == connect(_socket, (sockaddr*)&sinServer, sinLen))
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
	if (SOCKET_ERROR != _socket)
	{
#ifdef _WIN32
		if (SOCKET_ERROR == closesocket(_socket))
		{
			printf("Error:closesocket!\n");
			return -1;
		}
		else
		{
			printf("OK:closesocket!\n");
		}
#else
		if (SOCKET_ERROR == close(_socket))
		{
			printf("Error:close!\n");
			return -1;
		}
		else
		{
			printf("OK:close!\n");
		}
#endif // _WIN32

		_socket = SOCKET_ERROR;
	}
	
	return 0;
}

int Client::IsRun()
{
	return _socket == SOCKET_ERROR ? 0 : 1;
}

int Client::OnRun()
{
	if (IsRun())
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_socket, &fdRead);

		timeval tv = { 0, 0 };

		int ret = select(_socket + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:select!\n");
			Close();
			return -1;
		}

		if (FD_ISSET(_socket, &fdRead))
		{
			FD_CLR(_socket, &fdRead);

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
		return send(_socket, (const char*)pHeader, pHeader->msgLength, 0);

	return -1;
}

int Client::RecvData()
{
	int size = recv(_socket, _buffer, sizeof(MsgHeader), 0);
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

	MsgHeader* request = (MsgHeader*)_buffer;
	printf("--Recieve Msg Type:%d - Length:%d\n", request->msgType, request->msgLength);
	recv(_socket, _buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);

	switch (request->msgType)
	{
	case MSG_LOGIN_RES:
	{
		MsgLoginRes* login = (MsgLoginRes*)_buffer;
		printf("--Login Ret:%d\n", login->res);
	}
	break;
	case MSG_LOGOUT_RES:
	{
		MsgLogoutRes* logout = (MsgLogoutRes*)_buffer;
		printf("--Logout Ret:%d\n", logout->res);
	}
	break;
	case MSG_NEWUSER:
	{
		MsgNewUser* newUser = (MsgNewUser*)_buffer;
		printf("--New User <Socket=%d> Join Server!\n", newUser->user);
	}
	break;
	default:
	{

	}
	}

	return 0;
}