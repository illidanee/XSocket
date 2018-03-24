#include "XServer.h"

XServer::XServer()
{
	_Socket = INVALID_SOCKET;
}

XServer::~XServer()
{

}

int XServer::Init()
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

int XServer::Done()
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

int XServer::Open()
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

int XServer::Bind(const char* ip, unsigned short port)
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

int XServer::Listen(int n)
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

int XServer::Accept()
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
		std::shared_ptr<XReceiveServer> pLessServer = _AllServers[0];
		for (auto pServer : _AllServers)
		{
			if (pLessServer->GetClientNum() > pServer->GetClientNum())
			{
				pLessServer = pServer;
			}
		}

		std::shared_ptr<XClient> pClient(new XClient(client));
		pClient->Init(this, pLessServer.get());
		pLessServer->AddClient(std::shared_ptr<XClient>(pClient));

		OnClientJoin(pClient.get());
	}
	
	return 0;
}

int XServer::Start()
{
	for (int i = 0; i < _SERVER_SIZE_; ++i)
	{
		std::shared_ptr<XReceiveServer> pServer = std::make_shared<XReceiveServer>();
		pServer->SetNetEventObj(this);
		pServer->Start();
		_AllServers.push_back(pServer);
	}

	return 0;
}

int XServer::Close()
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

int XServer::IsRun()
{
	return _Socket == INVALID_SOCKET ? 0 : 1;
}

int XServer::OnRun()
{
	while (IsRun())
	{
		OnRunBegin();

		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_Socket, &fdRead);

		//设置10毫秒间隔，可以提高数据接受和发送select效率。
		timeval tv = { 0, 1000 };			
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

