#include "MyServer.h"

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

void MyServer::OnNetMsgRecv(XClient* pClient, MsgHeader* pMsgHeader)
{
	++_RecvPackageNum;
	++_PackageNum;

	//处理客户端请求
	switch (pMsgHeader->_MsgType)
	{
	case MSG_HEART:
	{
		pClient->ResetHeartTime();

		//使用任务系统
		MsgHeart* respond = new MsgHeart();
		std::function<void()> pTask = [pClient, respond]()
		{
			//XMariaDBConnect* connect = XMariaDB::GetInstance().GetConnect();
			//int num = 0;

			//if (connect)
			//	num = (int)connect->SearchStudent();
			//else
			//{
			//	printf("Error Conenct Data : -------------------  Task\n");
			//}

			//printf("------------------- Student = %d    \n", num);

			if (pClient->SendData(respond) < 0)
			{
				XInfo("<Client=%d Send Buffer Full!!!\n", (int)pClient->GetSocket());
			}

			//if (connect)
			//	XMariaDB::GetInstance().PushConnect(connect);
			//else
			//{
			//	printf("Error Conenct Data : ------------------- Task\n");
			//}

			delete respond;
		};

		pClient->GetServerObj()->AddTask(pTask);

		////直接发送数据
		//MsgHeart respond;
		//if (0 == pClient->SendData(&respond))
		//{
		//	XLog("<Client=%d Send Buffer Full!!!\n", (int)pClient->GetSocket());
		//}
	}
	break;
	case MSG_BYTESTREAM:
	{
		pClient->ResetHeartTime();

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
			char aa[32] = {};
			r.ReadArray(aa, 32);
			int bb[32] = {};
			r.ReadArray(bb, 32);

			XSendByteStream s(1024);
			s.WriteInt8(10);
			s.WriteInt16(20);
			s.WriteInt32(30);
			s.WriteInt64(40);
			s.WriteFloat(50.60f);
			s.WriteDouble(70.80);
			char a[] = "Server";
			s.WriteArray(a, (int)strlen(a));
			int b[] = { 1, 3, 5 };
			s.WriteArray(b, sizeof(b));
			s.Finish(MSG_BYTESTREAM);

			if (pClient->SendData((MsgHeader*)s.GetBuffer()) < 0)
			{
				XInfo("<Client=%d Send Buffer Full!!!\n", (int)pClient->GetSocket());
			}
		};

		pClient->GetServerObj()->AddTask(pTask);
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
