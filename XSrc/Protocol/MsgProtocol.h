#ifndef __MSG_PROTOCOL_H__
#define __MSG_PROTOCOL_H__

#include <stdint.h>

enum MGS_TYPE
{
	MSG_HEADER,
	MSG_ERROR,
	MSG_HEART,
	MSG_BYTESTREAM,
	MSG_LOGIN,
	MSG_LOGIN_RES,
};

struct MsgHeader
{
	int32_t _MsgLength;
	int32_t _MsgType;

	MsgHeader()
	{
		_MsgLength = sizeof(MsgHeader);
		_MsgType = MSG_HEADER;
	}
};

#endif