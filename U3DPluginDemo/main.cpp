#define __DLL_EXPORT__
#include "../U3DPlugin/Unity3DPlugin.h"

class Obj
{

public:
	static void OnMsg(void* pObj, MsgHeader* data, int len)
	{
		XRecvByteStream* pStream = CppRecvStreamCreate(data);
		int r1 = CppReadInt8(pStream, 0);
		printf("r1 = %d \n", r1);
	}
};

int main()
{
	SetLogPath("./client.log");

	Obj* pObj = new Obj();

	MyClient* pClient = Open(pObj, &Obj::OnMsg);
	Connect(pClient, "192.168.0.99", 9090);

	while (IsRun(pClient))
	{
		OnRun(pClient);

		XSendByteStream* pStream = CppSendStreamCreate(256);
		CppWriteInt8(pStream, 1);
		CppFinish(pStream);
		SendStream(pClient, pStream);
		CppSendStreamClose(pStream);

		Sleep(1000);
	}

	Close(pClient);

	delete pClient;

	return 0;
}