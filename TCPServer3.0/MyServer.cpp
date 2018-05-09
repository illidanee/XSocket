#include "MyServer.h"

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

void MyServer::OnClientJoin(std::shared_ptr<XClient> pClient)
{
	++_ClientNum;

	//MsgHeart msg;
	//pClient->SendData(&msg);
}

void MyServer::OnClientLeave(std::shared_ptr<XClient> pClient)
{
	--_ClientNum;
}

void MyServer::OnNetRecv(std::shared_ptr<XClient> pClient)
{
	++_RecvNum;
}

void MyServer::OnNetSend(std::shared_ptr<XClient> pClient)
{
	++_SendNum;
}

//这里地方的pMsgHeader地址上的内存不安全。随时可能被释放。如果假如任务系统延迟处理会有危险。
void MyServer::OnNetMsgRecv(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader)
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
		//持久化消息，防止消息被释放后再任务线程使用。
		char* pBuffer = new char[pMsgHeader->_MsgLength];
		memcpy(pBuffer, pMsgHeader, pMsgHeader->_MsgLength);

		std::function<void()> pTask = [pClient, pBuffer]()
		{
			XRecvByteStream r((MsgHeader*)pBuffer);

			int32_t type = MSG_ERROR;
			r.ReadInt32(type);

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
			int len7 = r.ReadArray(r7, 32);
			char r8[32] = {};
			int len8 = r.ReadArray(r8, 32);

			XSendByteStream s(1024);
			s.WriteInt8(10);
			s.WriteInt16(20);
			s.WriteInt32(30);
			s.WriteInt64(40);
			s.WriteFloat(55.66f);
			s.WriteDouble(77.88);
			s.WriteArray(r7, len7);
			s.WriteArray("Server", (int)strlen("Server"));
			s.Finish(MSG_BYTESTREAM);

			if (pClient->SendStream(&s) != 0)
			{
				XInfo("<Client=%d Send Buffer Full!!!\n", (int)pClient->GetSocket());
			}

			delete[] pBuffer;
		};

		pClient->GetServerObj()->AddTask(pTask);
	}
	break;
	case MSG_ENROLL:
	{
		//持久化消息，防止消息被释放后再任务线程使用。
		char* pBuffer = new char[pMsgHeader->_MsgLength];
		memcpy(pBuffer, pMsgHeader, pMsgHeader->_MsgLength);

		std::function<void()> pTask = [pClient, pBuffer]()
		{
			XSendByteStream s(1024);
			int ret = 0;

			//获取客户端发送的数据
			XRecvByteStream r((MsgHeader*)pBuffer);
			delete[] pBuffer;

			int32_t type = MSG_ERROR;
			r.ReadInt32(type);

			char pDevicel[64] = {};
			r.ReadArray(pDevicel, 64);
			char pSchool[64] = {};
			r.ReadArray(pSchool, 64);

			char pMajor[64] = {};
			r.ReadArray(pMajor, 64);
			char pStudentID[64] = {};
			r.ReadArray(pStudentID, 64);

			char pName[64] = {};
			r.ReadArray(pName, 64);
			char pPhoneNumber[64] = {};
			r.ReadArray(pPhoneNumber, 64);

			char pUserName[64] = {};
			r.ReadArray(pUserName, 64);
			char pPassword[64] = {};
			r.ReadArray(pPassword, 64);

			char pDeviceName[64] = {};
			r.ReadArray(pDeviceName, 64);
			char pDeviceType[64] = {};
			r.ReadArray(pDeviceType, 64);

			//获取数据库连接
			XMariaDBConnect* connect = XMariaDB::GetInstance().GetConnect();
			if (!connect)
			{
				printf("****** Error: XMariaDB::GetInstance().GetConnect() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_ENROLL_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			
			//查询是否已经注册
			int num = connect->SearchStudentByUserName(pUserName);
			if (num < 0)
			{
				printf("****** Error: SearchStudentByUserName() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_ENROLL_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num > 0)
			{
				printf("****** Error: SearchStudentByUserName() \n");
				ret = -2;
				s.WriteInt32(ret);
				s.Finish(MSG_ENROLL_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//查询是否存在信息
			num = connect->SearchStudentBySchoolAndStudentID(pSchool, pStudentID);
			if (num < 0)
			{
				printf("****** Error: SearchStudentBySchoolAndStudentID() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_ENROLL_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num == 0)
			{
				printf("****** Error: SearchStudentBySchoolAndStudentID() \n");
				ret = -3;
				s.WriteInt32(ret);
				s.Finish(MSG_ENROLL_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num > 1)
			{
				printf("****** Error: SearchStudentBySchoolAndStudentID() \n");
				ret = -4;
				s.WriteInt32(ret);
				s.Finish(MSG_ENROLL_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//更新数据库
			num = connect->UpdateStudentBySchoolAndStudentID(pDevicel, pSchool, pMajor, pStudentID, pName, pPhoneNumber, pUserName, pPassword, pDeviceName, pDeviceType);
			if (num != 1)
			{
				printf("****** Error: UpdateStudentBySchoolAndStudentID() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_ENROLL_RES);
				pClient->SendStream(&s);

				delete[] pBuffer;
				return;
			}

			s.WriteInt32(ret);
			s.Finish(MSG_ENROLL_RES);
			pClient->SendStream(&s);

			//delete[] pBuffer;
		};

		pClient->GetServerObj()->AddTask(pTask);
	}
	break;
	case MSG_LOGIN:
	{
		//持久化消息，防止消息被释放后再任务线程使用。
		char* pBuffer = new char[pMsgHeader->_MsgLength];
		memcpy(pBuffer, pMsgHeader, pMsgHeader->_MsgLength);

		std::function<void()> pTask = [pClient, pBuffer]()
		{
			XSendByteStream s(1024);
			int ret = 0;

			//获取客户端发送的数据
			XRecvByteStream r((MsgHeader*)pBuffer);
			delete[] pBuffer;

			int32_t type = MSG_ERROR;
			r.ReadInt32(type);

			char pDevicel[64] = {};
			r.ReadArray(pDevicel, 64);
			char pUserName[64] = {};
			r.ReadArray(pUserName, 64);
			char pPassword[64] = {};
			r.ReadArray(pPassword, 64);

			//获取数据库连接
			XMariaDBConnect* connect = XMariaDB::GetInstance().GetConnect();
			if (!connect)
			{
				printf("****** Error: XMariaDB::GetInstance().GetConnect() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_LOGIN_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//查询数据库
			int num = connect->SearchStudentByUserNameAndPassword(pUserName, pPassword);
			if (num < 0)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_LOGIN_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num == 0)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -2;
				s.WriteInt32(ret);
				s.Finish(MSG_LOGIN_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num > 1)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -3;
				s.WriteInt32(ret);
				s.Finish(MSG_LOGIN_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			s.WriteInt32(ret);
			s.Finish(MSG_LOGIN_RES);
			pClient->SendStream(&s);

			//查询推送消息 - 只处理成功情况。
			int msgID1 = 0;
			int num1 = connect->SearchMsgIDByUserName(pUserName, msgID1);
			int msgID2 = 0;
			char pMsg[2048] = {};
			int num2 = connect->SearchMsg(msgID2, pMsg);
			if (num1 == 1 && num2 == 1)
			{
				if (msgID1 < msgID2)
				{
					XSendByteStream sMsg(1024);
					sMsg.WriteArray(pMsg, strlen(pMsg));
					sMsg.Finish(MSG_BROADCAST_RES);
					pClient->SendStream(&sMsg);
				}
			}

			//delete[] pBuffer;
		};

		pClient->GetServerObj()->AddTask(pTask);
	}
	break;
	case MSG_SELFINFO:
	{
		//持久化消息，防止消息被释放后再任务线程使用。
		char* pBuffer = new char[pMsgHeader->_MsgLength];
		memcpy(pBuffer, pMsgHeader, pMsgHeader->_MsgLength);

		std::function<void()> pTask = [pClient, pBuffer]()
		{
			XSendByteStream s(1024);
			int ret = 0;

			//获取客户端发送的数据
			XRecvByteStream r((MsgHeader*)pBuffer);
			delete[] pBuffer;

			int32_t type = MSG_ERROR;
			r.ReadInt32(type);

			char pUserName[64] = {};
			r.ReadArray(pUserName, 64);
			char pPassword[64] = {};
			r.ReadArray(pPassword, 64);

			//获取数据库连接
			XMariaDBConnect* connect = XMariaDB::GetInstance().GetConnect();
			if (!connect)
			{
				printf("****** Error: XMariaDB::GetInstance().GetConnect() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_SELFINFO_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//查询个人信息
			char pSchool[64] = {};
			char pMajor[64] = {};
			char pStudentID[64] = {};
			char pName[64] = {};
			char pPhoneNumber[64] = {};
			int num = (int)connect->SearchInfoByUserName(pUserName, pSchool, pMajor, pStudentID, pName, pPhoneNumber);
			if (num < 0)
			{
				printf("****** Error: SearchInfoByUserName() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_SELFINFO_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num == 0)
			{
				printf("****** Error: SearchInfoByUserName() \n");
				ret = -2;
				s.WriteInt32(ret);
				s.Finish(MSG_SELFINFO_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num > 1)
			{
				printf("****** Error: SearchInfoByUserName() \n");
				ret = -3;
				s.WriteInt32(ret);
				s.Finish(MSG_SELFINFO_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			s.WriteInt32(ret);
			s.WriteArray(pSchool, strlen(pSchool));
			s.WriteArray(pMajor, strlen(pMajor));
			s.WriteArray(pStudentID, strlen(pStudentID));
			s.WriteArray(pName, strlen(pName));
			s.WriteArray(pPhoneNumber, strlen(pPhoneNumber));
			s.Finish(MSG_SELFINFO_RES);
			pClient->SendStream(&s);

			//delete[] pBuffer;
		};

		pClient->GetServerObj()->AddTask(pTask);
	}
	break;
	case MSG_FEEDBACK:
	{
		//持久化消息，防止消息被释放后再任务线程使用。
		char* pBuffer = new char[pMsgHeader->_MsgLength];
		memcpy(pBuffer, pMsgHeader, pMsgHeader->_MsgLength);

		std::function<void()> pTask = [pClient, pBuffer]()
		{
			XSendByteStream s(1024);
			int ret = 0;

			//获取客户端发送的数据
			XRecvByteStream r((MsgHeader*)pBuffer);
			delete[] pBuffer;

			int32_t type = MSG_ERROR;
			r.ReadInt32(type);

			char pUserName[64] = {};
			r.ReadArray(pUserName, 64);
			char pPassword[64] = {};
			r.ReadArray(pPassword, 64);
			char pContent[1024] = {};
			r.ReadArray(pContent, 1024);

			//获取数据库连接
			XMariaDBConnect* connect = XMariaDB::GetInstance().GetConnect();
			if (!connect)
			{
				printf("****** Error: XMariaDB::GetInstance().GetConnect() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_FEEDBACK_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//插入数据库
			int num = (int)connect->InsertFeedbackByUserName(pUserName, pContent);
			if (num < 0)
			{
				printf("****** Error: InsertFeedbackByUserName() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_FEEDBACK_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num == 0)
			{
				printf("****** Error: InsertFeedbackByUserName() \n");
				ret = -2;
				s.WriteInt32(ret);
				s.Finish(MSG_FEEDBACK_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			s.WriteInt32(ret);
			s.Finish(MSG_FEEDBACK_RES);
			pClient->SendStream(&s);

			//delete[] pBuffer;
		};

		pClient->GetServerObj()->AddTask(pTask);
	}
	break;
	case MSG_MODIFYPASSWORD:
	{
		//持久化消息，防止消息被释放后再任务线程使用。
		char* pBuffer = new char[pMsgHeader->_MsgLength];
		memcpy(pBuffer, pMsgHeader, pMsgHeader->_MsgLength);

		std::function<void()> pTask = [pClient, pBuffer]()
		{
			XSendByteStream s(1024);
			int ret = 0;

			//获取客户端发送的数据
			XRecvByteStream r((MsgHeader*)pBuffer);
			delete[] pBuffer;

			int32_t type = MSG_ERROR;
			r.ReadInt32(type);

			char pUserName[64] = {};
			r.ReadArray(pUserName, 64);
			char pPassword[64] = {};
			r.ReadArray(pPassword, 64);
			char pNewPasswrod[64] = {};
			r.ReadArray(pNewPasswrod, 64);

			//获取数据库连接
			XMariaDBConnect* connect = XMariaDB::GetInstance().GetConnect();
			if (!connect)
			{
				printf("****** Error: XMariaDB::GetInstance().GetConnect() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPASSWORD_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//查询数据库
			int num = (int)connect->SearchStudentByUserNameAndPassword(pUserName, pPassword);
			if (num < 0)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPASSWORD_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num == 0)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -2;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPASSWORD_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num > 1)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -3;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPASSWORD_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//更新数据库
			num = connect->UpdatePassword(pUserName, pPassword, pNewPasswrod);
			if (num != 1)
			{
				printf("****** Error: UpdatePassword() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPASSWORD_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			s.WriteInt32(ret);
			s.Finish(MSG_MODIFYPASSWORD_RES);
			pClient->SendStream(&s);

			//delete[] pBuffer;
		};

		pClient->GetServerObj()->AddTask(pTask);
	}
	break;
	case MSG_MODIFYPHONENUMBER:
	{
		//持久化消息，防止消息被释放后再任务线程使用。
		char* pBuffer = new char[pMsgHeader->_MsgLength];
		memcpy(pBuffer, pMsgHeader, pMsgHeader->_MsgLength);

		std::function<void()> pTask = [pClient, pBuffer]()
		{
			XSendByteStream s(1024);
			int ret = 0;

			//获取客户端发送的数据
			XRecvByteStream r((MsgHeader*)pBuffer);
			delete[] pBuffer;

			int32_t type = MSG_ERROR;
			r.ReadInt32(type);

			char pUserName[64] = {};
			r.ReadArray(pUserName, 64);
			char pPassword[64] = {};
			r.ReadArray(pPassword, 64);
			char pNewPhoneNumber[64] = {};
			r.ReadArray(pNewPhoneNumber, 64);

			//获取数据库连接
			XMariaDBConnect* connect = XMariaDB::GetInstance().GetConnect();
			if (!connect)
			{
				printf("****** Error: XMariaDB::GetInstance().GetConnect() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPHONENUMBER_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//查询数据库
			int num = (int)connect->SearchStudentByUserNameAndPassword(pUserName, pPassword);
			if (num < 0)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPHONENUMBER_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num == 0)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -2;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPHONENUMBER_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}
			else if (num > 1)
			{
				printf("****** Error: SearchStudentByUserNameAndPassword() \n");
				ret = -3;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPHONENUMBER_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			//更新数据库
			num = connect->UpdatePhoneNumber(pUserName, pPassword, pNewPhoneNumber);
			if (num != 1)
			{
				printf("****** Error: UpdatePassword() \n");
				ret = -1;
				s.WriteInt32(ret);
				s.Finish(MSG_MODIFYPHONENUMBER_RES);
				pClient->SendStream(&s);

				//delete[] pBuffer;
				return;
			}

			s.WriteInt32(ret);
			s.Finish(MSG_MODIFYPHONENUMBER_RES);
			pClient->SendStream(&s);

			//delete[] pBuffer;
		};

		pClient->GetServerObj()->AddTask(pTask);
	}
	break;
	case MSG_BROADCAST:
	{
	}
	break;
	default:
	{
		XInfo("Warn： default Msg。");
	}
	}
}

void MyServer::OnNetMsgDone(std::shared_ptr<XClient> pClient, MsgHeader* pMsgHeader)
{
	++_DonePackageNum;
	--_PackageNum;
}
