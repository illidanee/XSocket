#define __NO_DLL_INS__
#include "../U3DPlugin/Unity3DPlugin.h"

class Obj
{

public:
	static void OnMsg(void* pObj, MsgHeader* data)
	{
		XRecvByteStream* pStream = CppRecvStreamCreate(data);
		int r1 = CppReadInt8(pStream, 0);
		printf("r1 = %d \n", r1);
	}
	static void OnLeave(void* pObj)
	{

	}
};

int main()
{
	SetLogPath("./client.log");

	Obj* pObj = new Obj();

	MyClient* pClient = Open(pObj, &Obj::OnMsg, &Obj::OnLeave);
	bool ret = Connect(pClient, "192.168.0.90", 9091);

	XSendByteStream* pStream = CppSendStreamCreate(256);
	CppWriteInt8(pStream, 1);
	CppFinish(pStream, MGS_TYPE::MSG_BYTESTREAM);
	SendStream(pClient, pStream);
	CppSendStreamClose(pStream);

	while (IsRun(pClient))
	{
		OnRun(pClient);
		Sleep(1000);
	}

	Close(pClient);

	return 0;
}