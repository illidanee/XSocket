﻿#include "MyServer.h"

#include "../XSrc/MariaDB/XMariaDB.h"

MyServer::MyServer()
{
	//初始化其他
	_Timer.XInit();
	_ClientNum = 0;
	_RecvNum = 0;
	_SendNum = 0;
	_RecvPackageNum = 0;
	_DonePackageNum = 0;
	_PackageNum = 0;
}

MyServer::~MyServer()
{
	_Timer.XDone();
}

void MyServer::OnRunLoopBegin()
{
	if (_Timer.GetTime() > 1.0)
	{
		XInfo("| Client Num = %7d  | Recv Num = %7d  | Send Num = %7d  | RecvPackage Num = %7d  | DonePackage Num = %7d  | Package Num = %7d  |\n", (int)_ClientNum, (int)_RecvNum, (int)_SendNum, (int)_RecvPackageNum, (int)_DonePackageNum, (int)_PackageNum);
		_RecvNum = 0;
		_SendNum = 0;
		_RecvPackageNum = 0;
		_DonePackageNum = 0;
		_Timer.UpdateTime();
	}
}

void MyServer::OnClientJoin(XClient* pClient)
{
	++_ClientNum;

	//MsgHeart msg;
	//pClient->SendData(&msg);
}

void MyServer::OnClientLeave(XClient* pClient)
{
	--_ClientNum;
}

void MyServer::OnNetRecv(XClient* pClient)
{
	++_RecvNum;
}

void MyServer::OnNetSend(XClient* pClient)
{
	++_SendNum;
}

//这里地方的pMsgHeader地址上的内存不安全。随时可能被释放。如果假如任务系统延迟处理会有危险。
void MyServer::OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader)
{
	pClient->ResetHeartTime();

	++_RecvPackageNum;
	++_PackageNum;

	//处理客户端请求
	switch (pMsgHeader->_MsgType)
	{
	case MSG_ERROR:
	{
		// ...
	}
	break;
	case MSG_HEART:
	{
		// ...
	}
	break;
	case MSG_BYTESTREAM:
	{
		std::function<void()> pTask = [pClient, pMsgHeader]()
		{
			XRecvByteStream r(pMsgHeader);
			int8_t r1;
			r.ReadInt8(r1);
			int16_t r2;
			r.ReadInt16(r2);
			int32_t r3;
			r.ReadInt32(r3);
			int64_t r4;
			r.ReadInt64(r4);
			float r5;
			r.ReadFloat(r5);
			double r6;
			r.ReadDouble(r6);
			char r7[32] = {};
			r.ReadArray(r7, 32);

			//XInfo("r1 = %d, r2 = %d, r3 = %d, r4 = %I64d, r5 = %f, r6 = %f, r7 = %s \n", r1, r2, r3, r4, r5, r6, r7);

			XSendByteStream s(1024);
			s.WriteInt8(10);
			s.WriteInt16(20);
			s.WriteInt32(30);
			s.WriteInt64(40);
			s.WriteFloat(55.66f);
			s.WriteDouble(77.88);
			s.WriteArray("Server", (int)strlen("Server"));
			s.Finish(MSG_BYTESTREAM);
			
			if (pClient->SendStream(&s) != 0)
			{
				XInfo("<Client=%d Send Buffer Full!!!\n", (int)pClient->GetSocket());
			}
		};

		pClient->GetServerObj()->AddTask(pTask);
	}
	break;
	case MSG_LOGIN:
	{

	}
	break;
	default:
	{
		XInfo("Warn： default Msg。");
	}
	}
}

void MyServer::OnNetMsgDone(XClient* pClient, MsgHeader* pMsgHeader)
{
	++_DonePackageNum;
	--_PackageNum;
}
