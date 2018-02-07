#include <iostream>
#include <thread>


#include "Client.h"


void CmdThread(Client* client)
{
	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "Login"))
		{
			MsgLogin login = {};
			memcpy(login.name, "illidan", sizeof("illidan"));
			memcpy(login.pwd, "12345", sizeof("12345"));
			client->SendData(&login);
		}
		else if (0 == strcmp(buffer, "Logout"))
		{
			MsgLogout logout = {};
			memcpy(logout.name, "illidan", sizeof("illidan"));
			client->SendData(&logout);
		}
		else if (0 == strcmp(buffer, "Exit"))
		{
			client->Close();
			break;
		}
	}
}

int main()
{

	Client client;
	client.Init();
	client.Connect("192.168.0.90", 9090);

	Client client2;
	client2.Init();
	client2.Connect("192.168.0.90", 9091);

	std::thread t1(CmdThread, &client);
	t1.detach();

	std::thread t2(CmdThread, &client2);
	t2.detach();

	while (client.IsRun() || client2.IsRun())
	{
		client.OnRun();
		client2.OnRun();
	}

	client.Done();
	client2.Done();

	return 0;
}
