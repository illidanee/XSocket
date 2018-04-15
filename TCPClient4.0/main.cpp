#include "../XSrc/XCommon.h"
#include "MyClient.h"

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
	}

	//client.Close();

	return 0;
}