#include "XSendByteStream.h"

XSendByteStream::XSendByteStream(char* pBuffer, size_t nSize, bool bDelete)
	:
	XByteStream(pBuffer, nSize, bDelete)
{
	SetWriteOffset(sizeof(MsgHeader));
}

XSendByteStream::XSendByteStream(size_t nSize)
	:
	XByteStream(nSize)
{
	SetWriteOffset(sizeof(MsgHeader));
}

void XSendByteStream::Finish(MGS_TYPE type)
{
	size_t curoffset = GetWriteOffset();
	SetWriteOffset(0);
	WriteInt32(type);
	WriteInt32((int32_t)curoffset);
}