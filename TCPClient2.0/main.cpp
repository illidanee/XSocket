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
			memcpy(login._Name, "illidan", sizeof("illidan"));
			memcpy(login._Pwd, "12345", sizeof("12345"));
			client->SendData(&login);
		}
		else if (0 == strcmp(buffer, "Logout"))
		{
			MsgLogout logout = {};
			memcpy(logout._Name, "illidan", sizeof("illidan"));
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

	Client* client = new Client;
	client->Init();
	client->Open();
	client->Connect("192.168.0.99", 9090);
	//client->Connect("198.13.41.222", 9090);

	std::thread t1(CmdThread, client);
	t1.detach();

	MsgLogin login;
	memcpy(login._Name, "illidan", sizeof("illidan"));
	memcpy(login._Pwd, "12345", sizeof("12345"));

	while (client->IsRun())
	{
		client->SendData(&login);
		client->OnRun();
	}

	client->Close();
	client->Done();

	return 0;
}
