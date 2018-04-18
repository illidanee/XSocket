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
	int32_t _MsgLength;
	int32_t _MsgType;

	MsgHeader()
	{
		_MsgLength = sizeof(MsgHeader);
		_MsgType = MSG_HEADER;
	}
};

struct MsgError : public MsgHeader
{
	MsgError()
	{
		_MsgLength = sizeof(MsgError);
		_MsgType = MSG_ERROR;
	}
};

struct MsgHeart : public MsgHeader
{
	MsgHeart()
	{
		_MsgLength = sizeof(MsgHeart);
		_MsgType = MSG_HEART;
	}
};

#endif