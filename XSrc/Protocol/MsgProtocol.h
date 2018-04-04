#ifndef __MSG_PROTOCOL_H__
#define __MSG_PROTOCOL_H__

enum MGS_TYPE
{
	MSG_HEADER,
	MSG_ERROR,
	MSG_HEART,
	MSG_LOGIN,
	MSG_LOGIN_RES,
	MSG_LOGOUT,
	MSG_LOGOUT_RES,
	MSG_NEWUSER
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

struct MsgLogin : public MsgHeader
{
	MsgLogin()
	{
		_MsgType = MSG_LOGIN;
		_MsgLength = sizeof(MsgLogin);
	}
	char _Name[32];
	char _Pwd[32];
	char data[28];
};

struct MsgLoginRes : public MsgHeader
{
	MsgLoginRes()
	{
		_MsgType = MSG_LOGIN_RES;
		_MsgLength = sizeof(MsgLoginRes);
		_Ret = 0;
	}
	int _Ret;
	char data[88];
};

struct MsgLogout : public MsgHeader
{
	MsgLogout()
	{
		_MsgType = MSG_LOGOUT;
		_MsgLength = sizeof(MsgLogout);
	}
	char _Name[32];
};

struct MsgLogoutRes : public MsgHeader
{
	MsgLogoutRes()
	{
		_MsgType = MSG_LOGOUT_RES;
		_MsgLength = sizeof(MsgLogoutRes);
		_Ret = 0;
	}
	int _Ret;
};

struct MsgNewUser : public MsgHeader
{
	MsgNewUser()
	{
		_MsgType = MSG_NEWUSER;
		_MsgLength = sizeof(MsgNewUser);
		_UserID = 0;
	}
	int _UserID;
};

#endif