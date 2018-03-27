#include "XServer.h"

XServer::XServer()
	:
	_Socket(INVALID_SOCKET),
	_Run(false)
{
}

XServer::~XServer()
{
}

int XServer::Start()
{
	XLog("XServer:Start() Begin\n");

	//初始化服务器
	Init();
	Open();
	Bind(NULL, 9090);
	Listen(1000);

	//开启服务线程
	for (int i = 0; i < _SERVER_SIZE_; ++i)
	{
		//1-不使用对象池。
		//std::shared_ptr<XReceiveServer> pServer = std::make_shared<XReceiveServer>();
		//2-使用对象池。
		std::shared_ptr<XReceiveServer> pServer(new XReceiveServer(i));

		pServer->Init(this);
		pServer->Start();
		_AllServers.push_back(pServer);
	}

	//开始服务器
	_Run = true;

	XLog("XServer:Start() End\n");
	return 0;
}

int XServer::Stop()
{
	XLog("XServer:Stop() Begin\n");

	//关闭服务器 - 注意顺序。
	_Run = false;
	_Signal.Wait();

	//关闭服务线程
	for (auto iter : _AllServers)
	{	
		iter->Stop();
		iter->Done();
	}
	_AllServers.clear();

	//销毁服务器
	Close();
	Done();

	XLog("XServer:Stop() End\n");
	return 0;
}

int XServer::OnRun()
{
	XLog("XServer:OnRun() Begin\n");

	while (_Run)
	{
		OnRunLoopBegin();

		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_Socket, &fdRead);

		//设置10毫秒间隔，可以提高数据接受和发送select效率。
		timeval tv = { 0, 1000 };
		int ret = select((int)_Socket + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			XLog("Error<Socket=%d>:Select!\n", (int)_Socket);
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

	XLog("XServer:OnRun() End\n");
	_Signal.Wake();
	return 0;
}

int XServer::Init()
{
	//初始化网络环境
#ifdef _WIN32
	WORD wsaVersion = MAKEWORD(2, 2);
	WSADATA wsaData = {};
	int iRet = WSAStartup(wsaVersion, &wsaData);
	if (iRet)
	{
		XLog("Error:WSAStartup!\n");
		return -1;
	}
	else
	{
		XLog("OK:WSAStartup!\n");
	}
#endif

	return 0;
}

int XServer::Done()
{
	//销毁网络环境
#ifdef _WIN32
	int iRet = WSACleanup();
	if (SOCKET_ERROR == iRet)
	{
		XLog("Error:WSACleanup!\n");
		return -1;
	}
	else
	{
		XLog("OK:WSACleanup!\n");
	}
#endif

	return 0;
}

int XServer::Open()
{
	assert(INVALID_SOCKET == _Socket);

	_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _Socket)
	{
		XLog("Error:socket!\n");
		return -1;
	}
	else
	{
		XLog("OK<Socket=%d>:socket!\n", (int)_Socket);
	}

	return 0;
}

int XServer::Close()
{
	assert(INVALID_SOCKET != _Socket);

	if (INVALID_SOCKET != _Socket)
	{
#ifdef _WIN32
		if (SOCKET_ERROR == closesocket(_Socket))
		{
			XLog("Error<Socket=%d>:closesocket!\n", (int)_Socket);
		}
		else
		{
			XLog("OK<Socket=%d>:closesocket!\n", (int)_Socket);
		}
#else
		if (SOCKET_ERROR == close(_Socket))
		{
			XLog("Error<Socket=%d>:close!\n", (int)_Socket);
		}
		else
		{
			XLog("OK<Socket=%d>:close!\n", (int)_Socket);
		}
#endif
		_Socket = INVALID_SOCKET;
	}

	return 0;
}

int XServer::Bind(const char* ip, unsigned short port)
{
	assert(INVALID_SOCKET != _Socket);

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
	int iRet = bind(_Socket, (sockaddr*)&sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == iRet)
	{
		XLog("Error<Socket=%d>:bind!\n", (int)_Socket);
		return -1;
	}
	else
	{
		XLog("OK<Socket=%d>:bind!\n", (int)_Socket);
	}

	return 0;
}

int XServer::Listen(int n)
{
	assert(INVALID_SOCKET != _Socket);

	int iRet = listen(_Socket, n);
	if (SOCKET_ERROR == iRet)
	{
		XLog("Error<Socket=%d>:listen!\n", (int)_Socket);
		return -1;
	}
	else
	{
		XLog("OK<Socket=%d>:listen!\n", (int)_Socket);
	}

	return 0;
}

int XServer::Accept()
{
	assert(INVALID_SOCKET != _Socket);

	sockaddr_in sinClient = {};
	int sinLen = sizeof(sockaddr_in);
#ifdef _WIN32
	SOCKET client = accept(_Socket, (sockaddr*)&sinClient, &sinLen);
#else
	SOCKET client = accept(_Socket, (sockaddr*)&sinClient, (socklen_t*)&sinLen);
#endif
	if (client == INVALID_SOCKET)
	{
		XLog("Error<Socket=%d>:accept!\n", (int)_Socket);
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
	}
	
	return 0;
}


