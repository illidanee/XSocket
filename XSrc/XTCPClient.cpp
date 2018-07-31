#include "XTCPClient.h"

XTCPClient::XTCPClient()
	:
	_Client(nullptr),
	_bRun(false)
{
}

XTCPClient::~XTCPClient()
{

}

bool XTCPClient::Open()
{
	XNet::Go();

	assert(_Client == nullptr);

	SOCKET _Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == _Socket)
	{
		XError("socket!\n");
		return false;
	}

	_Client.reset(new XClient(this, this, _Socket));

	return true;
}

bool XTCPClient::Connect(const char* ip, unsigned short port)
{
	assert(_Client != nullptr);

	//优化：已经连接的时候，不在连接。此时如果执行connect会返回SOCKET_ERROR。
	if (_bRun)
		return true;

	sockaddr_in sinServer = {};
	sinServer.sin_family = AF_INET;
#ifdef _WIN32
	sinServer.sin_addr.S_un.S_addr = inet_addr(ip);
#else
	sinServer.sin_addr.s_addr = inet_addr(ip);
#endif // _WIN32
	sinServer.sin_port = htons(port);
	int sinLen = sizeof(sockaddr_in);
	if (SOCKET_ERROR == connect(_Client->GetSocket(), (sockaddr*)&sinServer, sinLen))
	{
		XError("connect!\n");
		return false;
	}

	OnClientJoin(_Client->GetSharedPtr());

	_bRun = true;
	return true;
}

void XTCPClient::Disconnect()
{
	OnClientLeave(_Client->GetSharedPtr());

	_bRun = false;
}

void XTCPClient::Close()
{
	assert(_Client != nullptr);

	if (_Client != nullptr)
	{
		_Client.reset();
		_Client = nullptr;
	}
}

bool XTCPClient::IsRun()
{
	return _bRun;
}

void XTCPClient::OnRun()
{
	OnRunBegin();

	SOCKET _Socket = _Client->GetSocket();

	_FdRead.Zero();
	_FdWrite.Zero();

	_FdRead.Add(_Socket);

	timeval tv = { 0, 0 };

	int ret = 0;
	if (_Client->HasData())
	{
		_FdWrite.Add(_Socket);
		ret = select((int)_Socket + 1, _FdRead.GetFdSet(), _FdWrite.GetFdSet(), nullptr, &tv);
	}
	else
	{
		ret = select((int)_Socket + 1, _FdRead.GetFdSet(), nullptr, nullptr, &tv);
	}
	
	if (SOCKET_ERROR == ret)
	{
		XError("select!\n");
		Disconnect();
		return;
	}
	else if ( 0 == ret )
	{
		return;
	}

	//接收消息
	if (_FdRead.Has(_Socket))
	{
		int ret = _Client->RecvMsg();
		if (ret < 0)
		{
			Disconnect();
		}
	}

	//发送消息
	if (_FdWrite.Has(_Socket))
	{
		int ret = _Client->SendMsg();
		if (ret < 0)
		{
			Disconnect();
		}
	}

	//处理消息
	_Client->DoMsg();
}

int XTCPClient::SendData(MsgHeader* pHeader)
{
	return _Client->SendMsg(pHeader);
}

int XTCPClient::SendStream(XByteStream* pStream)
{
	return _Client->SendStream(pStream);
}