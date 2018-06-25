#include "XTCPServer.h"

XTCPServer::XTCPServer()
	:
	_Socket(INVALID_SOCKET)
{
}

XTCPServer::~XTCPServer()
{
}

int XTCPServer::Start()
{
	XInfo("---------------------------------------------------------------------------------------------------- XServer:Start() Begin \n");

	//初始化网络环境
	XNet::Go();

	//初始化服务器
	Open();
	Bind(NULL, 9090);
	Listen(1000);

	//开启服务线程
	for (int i = 0; i < _SERVER_SIZE_; ++i)
	{
		//1-不使用对象池。
		//std::shared_ptr<XReceiveServer> pServer = std::make_shared<XReceiveServer>();
		//2-使用对象池。
		std::shared_ptr<XServer> pServer(new XServer(i));

		pServer->Init(this);
		pServer->Start();
		_AllServers.push_back(pServer);
	}

	//开始监听线程
	_Thread.Start(
		nullptr,
		[this](XThread* pThread) {
			OnRun(pThread);
		},
		nullptr
	);

	XInfo("---------------------------------------------------------------------------------------------------- XServer:Start() End \n");
	return 0;
}

int XTCPServer::Stop()
{
	XInfo("---------------------------------------------------------------------------------------------------- XServer:Stop() Begin \n");

	//关闭监听线程
	_Thread.Stop();

	//关闭服务线程
	for (auto iter : _AllServers)
	{	
		iter->Stop();
		iter->Done();
	}
	_AllServers.clear();

	//销毁服务器
	Close();

	XInfo("---------------------------------------------------------------------------------------------------- XServer:Stop() End \n");
	return 0;
}

void XTCPServer::Open()
{
	assert(INVALID_SOCKET == _Socket);

	_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _Socket)
	{
		XError("socket!\n");
	}
	else
	{
		XInfo("<Socket=%d>:socket!\n", (int)_Socket);
	}
}

void XTCPServer::Bind(const char* ip, unsigned short port)
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
		XError("<Socket=%d>:bind!\n", (int)_Socket);
	}
	else
	{
		XInfo("<Socket=%d>:bind!\n", (int)_Socket);
	}
}

void XTCPServer::Listen(int n)
{
	assert(INVALID_SOCKET != _Socket);

	int iRet = listen(_Socket, n);
	if (SOCKET_ERROR == iRet)
	{
		XError("<Socket=%d>:listen!\n", (int)_Socket);
	}
	else
	{
		XInfo("<Socket=%d>:listen!\n", (int)_Socket);
	}
}

void XTCPServer::Close()
{
	assert(INVALID_SOCKET != _Socket);

	if (INVALID_SOCKET != _Socket)
	{
#ifdef _WIN32
		if (SOCKET_ERROR == closesocket(_Socket))
		{
			XError("<Socket=%d>:closesocket!\n", (int)_Socket);
		}
		else
		{
			XInfo("<Socket=%d>:closesocket!\n", (int)_Socket);
		}
#else
		if (SOCKET_ERROR == close(_Socket))
		{
			XError("<Socket=%d>:close!\n", (int)_Socket);
		}
		else
		{
			XInfo("<Socket=%d>:close!\n", (int)_Socket);
		}
#endif
		_Socket = INVALID_SOCKET;
	}
}

void XTCPServer::Accept()
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
		XError("<Socket=%d>:accept!\n", (int)_Socket);
	}
	else
	{
		std::shared_ptr<XServer> pLessServer = _AllServers[0];
		for (auto pServer : _AllServers)
		{
			if (pLessServer->GetClientNum() > pServer->GetClientNum())
			{
				pLessServer = pServer;
			}
		}
		std::shared_ptr<XClient> pClient(new XClient(client, this, pLessServer.get()));
		pLessServer->AddClient(std::shared_ptr<XClient>(pClient));
	}
}

void XTCPServer::OnRun(XThread* pThread)
{
	XInfo("---------------------------------------------------------------------------------------------------- XServer:OnRun() Begin\n");

	while (pThread->IsRun())
	{
		OnRunLoopBegin();

		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(_Socket, &fdRead);

		//设置10毫秒间隔，可以提高数据接受和发送select效率。
		timeval tv = { 0, 1 };
		int ret = select((int)_Socket + 1, &fdRead, NULL, NULL, &tv);
		if (SOCKET_ERROR == ret)
		{
			XError("<Socket=%d>:Select!\n", (int)_Socket);
			pThread->Exit();
			break;
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

	XInfo("---------------------------------------------------------------------------------------------------- XServer:OnRun() End\n");
}




