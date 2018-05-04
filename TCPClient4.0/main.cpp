#include "../XSrc/XCommon.h"
#include "MyClient.h"
#include "../XSrc/XSendByteStream.h"

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

		//���Է�������
		XSendByteStream s(1024);
		s.WriteInt8(1);
		s.WriteInt16(2);
		s.WriteInt32(3);
		s.WriteInt64(4);
		s.WriteFloat(5.6f);
		s.WriteDouble(7.8);
		char b[] = { 'a', 'b', 'c', 'd', 'e' };
		s.WriteArray(b, sizeof(b));
		s.WriteArray("client", (int)strlen("client"));
		s.Finish(MSG_BYTESTREAM);
		client.SendStream(&s);

		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}

	client.Close();

	return 0;
}