#ifndef __XSENDBYTESTREAM_H__
#define __XSENDBYTESTREAM_H__

#include "../XCommon.h"
#include "XByteStream.h"

class XSendByteStream : public XByteStream
{
public:
	XSendByteStream(char* pBuffer, size_t nSize, bool bDelete = true);
	XSendByteStream(size_t nSize);

	void Finish(MGS_TYPE type);
};

#endif