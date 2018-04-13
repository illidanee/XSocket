#include "XBuffer.h"

XBuffer::XBuffer(int nSize)
	:
	_nSize(nSize),
	_pBuffer(nullptr),
	_nOffset(0)
{
	Init();
}

XBuffer::~XBuffer()
{
	Done();
}

void XBuffer::Init()
{
	if (!_pBuffer)
	{
		_pBuffer = new char[_nSize];
	}
}

void XBuffer::Done()
{
	if (_pBuffer)
	{
		delete[] _pBuffer;
		_pBuffer = nullptr;
	}
}

int XBuffer::Recv(SOCKET socket)
{
	int num = _nSize - _nOffset;
	if (num <= 0)
	{
		//���ջ������Ѿ�����
		return 1;
	}

	int size = recv(socket, _pBuffer + _nOffset, num, 0);
	if (size == -1)
	{
		//Socket����
		return -1;
	}
	else if (size == 0)
	{
		//�Զ˶Ͽ���
		return -2;
	}

	//�������ա�
	_nOffset += size;

	return 0;
}

int XBuffer::Send(SOCKET socket)
{
	if (_nOffset <= 0)
	{
		//���ͻ�����Ϊ��
		return 1;
	}

	int size = send(socket, _pBuffer, _nOffset, 0);
	if (size == -1)
	{
		//Socket����
		return -1;
	}
	else if (size == 0)
	{
		//�Զ˶Ͽ���
		return -2;
	}

	//�������͡�
	_nOffset = 0;

	return 0;
}

int XBuffer::Push(MsgHeader* pMsg)
{
	if (_nOffset + pMsg->_MsgLength > _nSize)
	{
		//���ͻ��������ˡ�
		return -1;
	}

	memcpy(_pBuffer + _nOffset, pMsg, pMsg->_MsgLength);
	_nOffset += pMsg->_MsgLength;

	return 0;
}

int XBuffer::Pop()
{
	if (_nOffset < sizeof(MsgHeader))
	{
		//��Ϣ���Ȳ�����Ϣͷ��
		return -1;
	}

	MsgHeader* pMsg = (MsgHeader*)_pBuffer;
	if (_nOffset < pMsg->_MsgLength)
	{
		//��Ϣ���Ȳ�����Ϣ�塣
		return -2;
	}

	int length = pMsg->_MsgLength;
	if (_nOffset > pMsg->_MsgLength)
		memcpy(_pBuffer, _pBuffer + pMsg->_MsgLength, _nOffset - pMsg->_MsgLength);

	_nOffset -= length;

	return 0;
}

bool XBuffer::HasMsg()
{
	if (_nOffset >= sizeof(MsgHeader))
	{
		MsgHeader* pMsg = (MsgHeader*)_pBuffer;
		if (_nOffset >= pMsg->_MsgLength)
			return true;
	}
	return false;
}

MsgHeader* XBuffer::Front()
{
	return (MsgHeader*)_pBuffer;
}
