#include "XClient.h"

XClient::XClient(SOCKET client)
{
	_pNetEventObj = nullptr;
	_pReceiveServerObj = nullptr;

	_Socket = client;
	memset(_RecvBuffer, 0, _RECV_BUFFER_SIZE_);
	_RecvStartPos = 0;
	memset(_SendBuffer, 0, _SEND_BUFFER_SIZE_);
	_SendStartPos = 0;

	_HeartTime = 0;
	_SendTime = 0;
}

XClient::~XClient()
{
	XLog("XClient:~XClient()\n");

	shutdown(_Socket, SD_BOTH);
#ifdef _WIN32
	closesocket(_Socket);
#else
	close(_Socket);
#endif
}

void XClient::Init(XIEvent* pEventObj, XReceiveServer* pReceiveServerObj)
{
	_pNetEventObj = pEventObj;
	_pReceiveServerObj = pReceiveServerObj;
}

int XClient::RecvData()
{
	//�������ݵ����ջ�������
	char* pBuffer = _RecvBuffer + _RecvStartPos;
	int size = recv(_Socket, pBuffer, _RECV_BUFFER_SIZE_ - _RecvStartPos, 0);
	if (SOCKET_ERROR == size)
	{
		//printf("OK:Client<Socket=%d> off!\n", (int)_Socket);
		return -1;
	}
	else if (size == 0)
	{
		//printf("OK:Client<Socket=%d> quit!\n", (int)_Socket);
		return -2;
	}

	_RecvStartPos += size;

	if (_pNetEventObj)
		_pNetEventObj->OnNetRecv(this);

	//���ݻ��������ȴ�����Ϣͷ����
	while (_RecvStartPos >= sizeof(MsgHeader))
	{
		MsgHeader* pHeader = (MsgHeader*)_RecvBuffer;
		//���ݻ��������ȴ�����Ϣ����
		if (_RecvStartPos >= pHeader->_MsgLength)
		{
			//���ݻ�����ʣ��δ�������ݳ���
			int len = _RecvStartPos - pHeader->_MsgLength;

			if (_pNetEventObj)
				_pNetEventObj->OnNetMsgRecv(this, pHeader, _pReceiveServerObj);

			//���ݻ�����ʣ��δ��������ǰ�� -- �˴�Ϊģ�⴦��
			memcpy(_RecvBuffer, _RecvBuffer + pHeader->_MsgLength, len);
			_RecvStartPos = len;
		}
		else
		{
			//���ݻ�����ʣ��δ�������ݲ���һ��������Ϣ
			break;
		}
	}

	return 0;
}

int XClient::SendData(MsgHeader* pHeader)
{
	int ret = 0;

	const char* pBuffer = (const char*)pHeader;
	int nSendBufferSize = pHeader->_MsgLength;

	if (_SendStartPos + nSendBufferSize <= _SEND_BUFFER_SIZE_)
	{
		memcpy(_SendBuffer + _SendStartPos, pBuffer, nSendBufferSize);
		_SendStartPos += nSendBufferSize;
		ret = nSendBufferSize;

		if (_pNetEventObj)
			_pNetEventObj->OnNetMsgDone(this, pHeader, _pReceiveServerObj);
	}

	return ret;
}

int XClient::SendData()
{
	int ret = 0;

	ResetSendTime();

	if (_SendStartPos > 0 && INVALID_SOCKET != _Socket)
	{
		ret = send(_Socket, _SendBuffer, _SendStartPos, 0);
		_SendStartPos = 0;

		if (_pNetEventObj)
			_pNetEventObj->OnNetSend(this);
	}

	return ret;
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