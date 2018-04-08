#ifndef __MSG_PROTOCOL_H__
#define __MSG_PROTOCOL_H__

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
	MGS_TYPE _MsgType;
	int _MsgLength;
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
	char _data[92];
};

#endif