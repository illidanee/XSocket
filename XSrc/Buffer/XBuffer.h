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
	int _nSize;						//缓冲区大小
	char* _pBuffer;					//缓冲区
	int _nOffset;					//缓冲区位置偏移
};

#endif