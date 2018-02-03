#define WIN32_LEAN_AND_MEAN

#include <vector>

#include <stdio.h>
#include <windows.h>
#include <WinSock2.h>

enum MGS_TYPE
{
	MSG_ERROR,
	MSG_LOGIN,
	MSG_LOGIN_RES,
	MSG_LOGOUT,
	MSG_LOGOUT_RES,
	MSG_NEWUSER
};

struct MsgHeader
{
	MGS_TYPE msgType;
	int msgLength;
};

struct MsgLogin : public MsgHeader
{
	MsgLogin()
	{
		msgType = MSG_LOGIN;
		msgLength = sizeof(MsgLogin);
	}
	char name[32];
	char pwd[32];
};

struct MsgLoginRes : public MsgHeader
{
	MsgLoginRes()
	{
		msgType = MSG_LOGIN_RES;
		msgLength = sizeof(MsgLoginRes);
		res = 0;
	}
	int res;
};

struct MsgLogout : public MsgHeader
{
	MsgLogout()
	{
		msgType = MSG_LOGOUT;
		msgLength = sizeof(MsgLogout);
	}
	char name[32];
};

struct MsgLogoutRes : public MsgHeader
{
	MsgLogoutRes()
	{
		msgType = MSG_LOGOUT_RES;
		msgLength = sizeof(MsgLogoutRes);
		res = 0;
	}
	int res;
};

struct MsgNewUser : public MsgHeader
{
	MsgNewUser()
	{
		msgType = MSG_NEWUSER;
		msgLength = sizeof(MsgNewUser);
		user = 0;
	}
	int user;
};

std::vector<SOCKET> g_AllClients;

int HandleMsg(SOCKET client)
{
	//���տͻ�������
	char buffer[1024] = {};
	int size = recv(client, buffer, sizeof(MsgHeader), 0);
	MsgHeader* request = (MsgHeader*)buffer;
	if (SOCKET_ERROR == size)
	{
		printf("Error:���տͻ���<Socket=%d>����!\n", client); //��Xǿ�йرա�
		return -1;
	}
	else if (size == 0)
	{
		printf("OK:�ͻ���<Socket=%d>�ر�����!\n", client);	 //��q����رա�
		return -2;
	}

	//��ʾ�ͻ�������
	printf("--���տͻ���<Socket=%d>����Type:%d - Length:%d\n", client, request->msgType, request->msgLength);

	//����ͻ�������
	switch (request->msgType)
	{
	case MSG_LOGIN:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLogin* login = (MsgLogin*)buffer;

		printf("--������Ϣ���ݣ�Name:%s - Pwd:%s\n", login->name, login->pwd);

		MsgLoginRes respond;
		send(client, (char*)&respond, sizeof(MsgLoginRes), 0);
	}
	break;
	case MSG_LOGOUT:
	{
		recv(client, buffer + sizeof(MsgHeader), request->msgLength - sizeof(MsgHeader), 0);
		MsgLogout* logout = (MsgLogout*)buffer;

		printf("--�ǳ���Ϣ���ݣ�Name:%s\n", logout->name);

		MsgLogoutRes respond;
		send(client, (char*)&respond, sizeof(MsgLogoutRes), 0);
	}
	break;
	default:
	{

	}
	}

	return 0;
}

int main()
{
	//������
	WORD wsaVersion = MAKEWORD(2, 2);
	WSADATA wsaData = {};
	int iErrorNo = WSAStartup(wsaVersion, &wsaData);
	if (iErrorNo)
	{
		printf("Error:WSA start up failed!\n");
	}

	//����Socket
	SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _socket)
	{
		printf("Error:����Socket!\n");
	}
	else
	{
		printf("OK:����Socket!\n");
	}

	//��IP�Ͷ˿�
	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(9090);
	if (SOCKET_ERROR == bind(_socket, (sockaddr*)&sin, sizeof(sockaddr_in)))
	{
		printf("Error:��IP�Ͷ˿�!\n");
	}
	else
	{
		printf("OK:��IP�Ͷ˿�!\n");
	}

	//��������
	if (SOCKET_ERROR == listen(_socket, 10))
	{
		printf("Error:��������!\n");
	}
	else
	{
		printf("OK:��������!\n");
	}

	while (true)
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_socket, &fdRead);

		for (size_t i = 0; i < g_AllClients.size(); ++i)
		{
			FD_SET(g_AllClients[i], &fdRead);
		}
		
		timeval tv = { 0, 0 };

		int ret = select(0, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			printf("Error:Select����!\n");
			break;
		}

		if (FD_ISSET(_socket, &fdRead))
		{
			FD_CLR(_socket, &fdRead);

			//�ȴ��ͻ�������
			sockaddr_in sinClient = {};
			int sinLen = sizeof(sockaddr_in);
			SOCKET _client = INVALID_SOCKET;
			_client = accept(_socket, (sockaddr*)&sinClient, &sinLen);
			if (_client == INVALID_SOCKET)
			{
				printf("Error:�ͻ���<Socket=%d>����!\n", _client);
			}
			else
			{
				printf("OK:�ͻ���<Socket=%d>����!\n", _client);

				//ת���ͻ���½��Ϣ
				MsgNewUser msgNewUser;
				msgNewUser.user = _client;

				for (std::vector<SOCKET>::iterator it = g_AllClients.begin(); it != g_AllClients.end(); ++it)
				{
					send(*it, (char*)&msgNewUser, sizeof(MsgNewUser), 0);
				}

				//��ӿͻ���
				g_AllClients.push_back(_client);
			}
		}

		for (unsigned int i = 0; i < fdRead.fd_count; ++i)
		{
			int ret = HandleMsg(fdRead.fd_array[i]);
			if (ret < 0)
			{
				//ɾ���ͻ���
				std::vector<SOCKET>::iterator iter = find(g_AllClients.begin(), g_AllClients.end(), fdRead.fd_array[i]);
				if (iter != g_AllClients.end())
					g_AllClients.erase(iter);

				//�رտͻ�������
				if (SOCKET_ERROR == closesocket(fdRead.fd_array[i]))
				{
					printf("Error:�رտͻ���<Socket=%d>����!\n", fdRead.fd_array[i]);
				}
				else
				{
					printf("OK:�رտͻ���<Socket=%d>����!\n", fdRead.fd_array[i]);
				}
			}
		}
	}

	//�رտͻ�������
	for (std::vector<SOCKET>::iterator it = g_AllClients.begin(); it != g_AllClients.end(); ++it)
	{
		//�رտͻ�������
		if (SOCKET_ERROR == closesocket(*it))
		{
			printf("Error:�رտͻ�������!\n");
		}
		else
		{
			printf("OK:�رտͻ�������!\n");
		}
	}
	g_AllClients.clear();

	//�ر�����
	int iError = WSACleanup();
	if (SOCKET_ERROR == iError)
	{
		printf("Error:WSA clean up failed!\n");
	}

	//�˳�
	getchar();
	return 0;
}