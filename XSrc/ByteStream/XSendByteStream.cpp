#include "XSendByteStream.h"

XSendByteStream::XSendByteStream(char* pBuffer, int nSize, bool bDelete)
	:
	XByteStream(pBuffer, nSize, bDelete)
{
	SetWriteOffset(sizeof(MsgHeader));
}

XSendByteStream::XSendByteStream(int nSize)
	:
	XByteStream(nSize)
{
	SetWriteOffset(sizeof(MsgHeader));
}

void XSendByteStream::Finish(MGS_TYPE type)
{
	int curoffset = GetWriteOffset(); 
	SetWriteOffset(0);	
	WriteInt32(curoffset);
	WriteInt32(type);
}