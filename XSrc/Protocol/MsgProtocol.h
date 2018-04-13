#ifndef __MSG_PROTOCOL_H__
#define __MSG_PROTOCOL_H__

#include <stdint.h>

enum MGS_TYPE
{
	MSG_HEADER,
	MSG_ERROR,
	MSG_HEART,
	MSG_BYTESTREAM
};

struct MsgHeader
{
	MsgHeader()
	{
		_MsgType = MSG_HEADER;
		_MsgLength = sizeof(MsgHeader);
	}
	int32_t _MsgType;
	int32_t _MsgLength;
};

struct MsgError : public MsgHeader
{
	MsgError()
	{
		_MsgType = MSG_ERROR;
		_MsgLength = sizeof(MsgError);
	}
};

struct MsgHeart : public MsgHeader
{
	MsgHeart()
	{
		_MsgType = MSG_HEART;
		_MsgLength = sizeof(MsgHeart);
	}
};

#endif