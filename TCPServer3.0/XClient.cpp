#include "XClient.h"

XClient::XClient(SOCKET client, XIEvent* pEventObj, XReceiveServer* pReceiveServerObj)
	:
	_pNetEventObj(nullptr),
	_pReceiveServerObj(nullptr),
	_Socket(client),
	_RecvBuffer(_RECV_BUFFER_SIZE_),
	_SendBuffer(_SEND_BUFFER_SIZE_),
	_HeartTime(0),
	_SendTime(0)
{
}

XClient::~XClient()
{
	//XLog("XClient:~XClient()\n");

	shutdown(_Socket, SD_BOTH);
#ifdef _WIN32
	closesocket(_Socket);
#else
	close(_Socket);
#endif
}

int XClient::RecvData()
{
	int nRet = _RecvBuffer.Recv(_Socket);
	if (nRet < 0)
		return  -1;

	if (nRet == 0)
	{
		if (_pNetEventObj)
			_pNetEventObj->OnNetRecv(this);

		while (_RecvBuffer.HasMsg())
		{
			if (_pNetEventObj)
				_pNetEventObj->OnNetMsgRecv(this, _RecvBuffer.Front(), _pReceiveServerObj);

			_RecvBuffer.Pop();
		}
	}

	return 0;
}

int XClient::SendData(MsgHeader* pHeader)
{
	int nRet = _SendBuffer.Push(pHeader);
	if (nRet < 0)
		return -1;

	if (_pNetEventObj)
		_pNetEventObj->OnNetMsgDone(this, pHeader, _pReceiveServerObj);

	return 0;
}

int XClient::SendData()
{
	int nRet = _SendBuffer.Send(_Socket);

	//有错误发生
	if (nRet < 0)
		return -1;

	//发送了数据
	if (nRet == 0)
	{
		if (_pNetEventObj)
			_pNetEventObj->OnNetSend(this);
	}

	ResetSendTime();

	return 0;
}

void XClient::ResetHeartTime()
{
	_HeartTime = 0;
}

bool XClient::CheckHeartTime(time_t t)
{
	_HeartTime += t;
	if (_HeartTime >= _XCLIENT_HEART_TIME_)
	{
		XLog("CheckHeartTime : Client<socket=%d> timeout on time = %d! \n", (int)_Socket, (int)_HeartTime);
		return true;
	}

	return false;
}

void XClient::ResetSendTime()
{
	_SendTime = 0;
}

void XClient::CheckSendTime(time_t t)
{
	_SendTime += t;
	if (_SendTime >= _XCLIENT_SEND_TIME_)
	{
		//XError("CheckSendTime! \n");
		SendData();
	}
}