#include "Client.h"

Client::Client()
{
	_Socket = INVALID_SOCKET;
	//memset(_RecvBuffer, 0, sizeof(_RecvBuffer));
	memset(_DataBuffer, 0, sizeof(_DataBuffer));
	_StartPos = 0;
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
#endif
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
#endif
	return 0;
}

int Client::Open()
{
	_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _Socket)
	{
		printf("Error:socket!\n");
		return -1;
	}

	return 0;
}

int Client::Connect(const char* ip, unsigned short port)
{
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
		return -1;
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
#else
		if (SOCKET_ERROR == close(_Socket))
		{
			printf("Error:close!\n");
			return -1;
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

int Client::SendData(MsgHeader* pHeader, int len)
{
	if (IsRun() && pHeader)
		return send(_Socket, (const char*)pHeader, len, 0);

	return -1;
}

int Client::RecvData()
{
	recv(_Socket, _DataBuffer, _BUFFER_SIZE_, 0);
	////接收数据到接收缓冲区中
	//char* pBuffer = _DataBuffer + _StartPos;
	//int size = recv(_Socket, pBuffer, _BUFFER_SIZE_ - _StartPos, 0);
	//if (SOCKET_ERROR == size)
	//{
	//	printf("OK:Server off!\n");
	//	return -1;
	//}
	//else if (size == 0)
	//{
	//	printf("OK:Server quit!\n");
	//	return -2;
	//}

	////将接收缓冲区数据拷贝到数据缓冲区
	////memcpy(_DataBuffer + _StartPos, _RecvBuffer, size);
	//_StartPos += size;

	////数据缓冲区长度大于消息头长度
	//while (_StartPos >= sizeof(MsgHeader))
	//{
	//	MsgHeader* pHeader = (MsgHeader*)_DataBuffer;
	//	//数据缓冲区长度大于消息长度
	//	if (_StartPos >= pHeader->_MsgLength)
	//	{
	//		//数据缓冲区剩余未处理数据长度
	//		int len = _StartPos - pHeader->_MsgLength;
	//		
	//		//处理消息
	//		OnNetMsg(pHeader);

	//		//数据缓冲区剩余未处理数据前移 -- 此处为模拟处理
	//		memcpy(_DataBuffer, _DataBuffer + pHeader->_MsgLength, len);
	//		_StartPos = len;
	//	}
	//	else
	//	{
	//		//数据缓冲区剩余未处理数据不够一条完整消息
	//		break;
	//	}
	//}

	return 0;
}

int Client::OnNetMsg(MsgHeader* pHeader)
{
	switch (pHeader->_MsgType)
	{
	case MSG_LOGIN_RES:
	{
		//MsgLoginRes* login = (MsgLoginRes*)pHeader;
		//printf("----Login Ret:%d\n", login->_Ret);
	}
	break;
	default:
	{
		//printf("----Recv Error Msg!\n");
	}
	}

	return 0;
}