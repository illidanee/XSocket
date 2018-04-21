#include "../XSrc/XCommon.h"
#include "MyClient.h"
#include "../XSrc/ByteStream/XSendByteStream.h"

int main()
{
	XLog::SetFile("./Client.log", "w");

	XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");
	XInfo("                                                               C++ Client                                                              \n");
	XInfo("                                                                                                        Designed by Org.illidan        \n");
	XInfo("---------------------------------------------------------------------------------------------------------------------------------------\n");


	MyClient client;
	client.Open();
	client.Connect("192.168.0.99", 9090);

	while (client.IsRun())
	{
		client.OnRun();

		//²âÊÔ·¢ËÍÊý¾Ý
		XSendByteStream* s = new XSendByteStream(1024);
		//s.WriteType(MGS_TYPE.MSG_BYTESTREAM);
		s->WriteInt8(1);
		s->WriteInt16(2);
		s->WriteInt32(3);
		s->WriteInt64(4);
		s->WriteFloat(5.6f);
		s->WriteDouble(7.8);
		s->WriteArray("client", strlen("client"));
		int b[] = { 1, 2, 3, 4, 5 };
		s->WriteArray(b, sizeof(b));
		s->Finish(MSG_BYTESTREAM);
		client.SendStream(s);
	}

	client.Close();

	return 0;
}