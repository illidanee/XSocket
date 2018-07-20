#include "XClient.h"

XClient::XClient(SOCKET client, XIGlobalEvent* pGlobalObj, XIServerEvent* pServerObj, int nHeartTime, int nSendTime, int nRecvBufferSize, int nSendBufferSize)
	:
	_Socket(client),
	_pGlobalObj(pGlobalObj),
	_pServerObj(pServerObj),
	_CurHeartTime(0),
	_HeartTime(nHeartTime),
	_CurSendTime(0),
	_SendTime(nSendTime),
	_RecvBuffer(nRecvBufferSize),
	_SendBuffer(nSendBufferSize)
{
}

XClient::~XClient()
{
#ifdef _WIN32
	shutdown(_Socket, SD_BOTH);
	closesocket(_Socket);
#else
	shutdown(_Socket, SHUT_RDWR);
	close(_Socket);
#endif
}

int XClient::RecvData()
{
	int nRet = _RecvBuffer.Recv(_Socket);
	if (nRet != 0)
		return  nRet;

	if (_pGlobalObj)
		_pGlobalObj->OnNetRecv(shared_from_this());

	while (_RecvBuffer.HasMsg())
	{
		if (_pGlobalObj)
			_pGlobalObj->OnNetMsgRecv(shared_from_this(), _RecvBuffer.Front());

		_RecvBuffer.Pop();
	}

	return nRet;
}

int XClient::SendData(MsgHeader* pHeader)
{
	int nRet = _SendBuffer.Push(pHeader);
	if (nRet != 0)
		return nRet;

	if (_pGlobalObj)
		_pGlobalObj->OnNetMsgDone(shared_from_this(), pHeader);

	return nRet;
}

int XClient::SendStream(XByteStream* pByteStream)
{
	return SendData((MsgHeader*)pByteStream->GetBuffer());
}

int XClient::SendData()
{
	int nRet = _SendBuffer.Send(_Socket);
	if (nRet != 0)
		return nRet;

	//发送了数据
	if (_pGlobalObj)
		_pGlobalObj->OnNetSend(shared_from_this());

	ResetSendTime();

	return 0;
}

bool XClient::HasMsg()
{
	return _SendBuffer.HasMsg();
}

bool XClient::HasData()
{
	return _SendBuffer.HasData();
}

void XClient::ResetHeartTime()
{
	_CurHeartTime = 0;
}

bool XClient::CheckHeartTime(time_t t)
{
	_CurHeartTime += t;
	if (_CurHeartTime >= _XCLIENT_HEART_TIME_)
	{
		//XInfo("CheckHeartTime : Client<socket=%d> timeout on time = %d! \n", (int)_Socket, (int)_HeartTime);
		return true;
	}

	return false;
}

void XClient::ResetSendTime()
{
	_CurSendTime = 0;
}

void XClient::CheckSendTime(time_t t)
{
	_CurSendTime += t;
	if (_CurSendTime >= _XCLIENT_SEND_TIME_)
	{
		//XInfo("CheckSendTime! \n");
		SendData();
	}
}

XIGlobalEvent* XClient::GetGlobalObj()
{
	return _pGlobalObj;
}

XIServerEvent* XClient::GetServerObj()
{
	return _pServerObj;
}

std::shared_ptr<XClient> XClient::GetSharedPrt()
{
	return shared_from_this();
}
