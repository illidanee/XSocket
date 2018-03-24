#include "XClient.h"

XClient::XClient(SOCKET client)
{
	_Socket = client;
	memset(_RecvBuffer, 0, _RECV_BUFFER_SIZE_);
	_RecvStartPos = 0;
	memset(_SendBuffer, 0, _SEND_BUFFER_SIZE_);
	_SendStartPos = 0;
}

XClient::~XClient()
{

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
	if (_pNetEventObj)
		_pNetEventObj->OnNetRecv(this);
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
	if (_pNetEventObj)
		_pNetEventObj->OnNetMsgDone(this, pHeader, _pReceiveServerObj);

	const char* pBuffer = (const char*)pHeader;
	int nSendBufferSize = pHeader->_MsgLength;

	while (true)
	{
		if (_SendStartPos + nSendBufferSize >= _SEND_BUFFER_SIZE_)
		{
			int len = _SEND_BUFFER_SIZE_ - _SendStartPos;
			memcpy(_SendBuffer + _SendStartPos, pBuffer, len);
			pBuffer += len;
			nSendBufferSize -= len;

			int ret = send(_Socket, _SendBuffer, _SEND_BUFFER_SIZE_, 0);
			if (_pNetEventObj)
				_pNetEventObj->OnNetSend(this);

			_SendStartPos = 0;

			if (SOCKET_ERROR == ret)
			{
				return -1;
			}
		}
		else
		{
			memcpy(_SendBuffer + _SendStartPos, pBuffer, nSendBufferSize);
			_SendStartPos += nSendBufferSize;
			break;
		}
	}

	return 0;
}