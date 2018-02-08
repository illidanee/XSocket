#include "Server.h"

int main()
{
	Server* server = new Server;
	server->Init();
	server->Open();
	server->Bind(NULL, 9090);
	server->Listen(10);

	while (server->IsRun())
	{
		server->OnRun();
	}

	server->CloseAll();
	server->Close();

	return 0;
}