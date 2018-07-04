#ifndef __MSG_PROTOCOL_H__
#define __MSG_PROTOCOL_H__

#include <stdint.h>

enum MGS_TYPE
{
	MSG_HEADER,
	MSG_ERROR,
	MSG_HEART,
	MSG_BYTESTREAM,
	MSG_ENROLL,
	MSG_ENROLL_RES,
	MSG_LOGIN,
	MSG_LOGIN_RES,
	MSG_SELFINFO,
	MSG_SELFINFO_RES,
	MSG_BOOKINFO,
	MSG_BOOKINFO_RES,
	MSG_FEEDBACK,
	MSG_FEEDBACK_RES,
	MSG_MODIFYPASSWORD,
	MSG_MODIFYPASSWORD_RES,
	MSG_MODIFYPHONENUMBER,
	MSG_MODIFYPHONENUMBER_RES,
	MSG_BROADCAST,
	MSG_BROADCAST_RES
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

struct MsgHeart : public MsgHeader
{
	MsgHeart()
	{
		_MsgLength = sizeof(MsgHeart);
		_MsgType = MSG_HEART;
	}
	int no;
	char name[16];
	char pwd[16];
	char other[56];
};

#endif