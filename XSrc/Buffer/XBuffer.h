#ifndef __XBUFFER_H__
#define __XBUFFER_H__

#include "../XCommon.h"

class XBuffer
{
public:
	XBuffer(int nSize);
	~XBuffer();

	void Init();
	void Done();

	int Recv(SOCKET socket);
	int Send(SOCKET socket);

	int Push(MsgHeader* pMsg);
	int Pop();
	bool HasMsg();
	MsgHeader* Front();

private:
	int _nSize;						//��������С
	char* _pBuffer;					//������
	int _nOffset;					//������λ��ƫ��
};

#endif