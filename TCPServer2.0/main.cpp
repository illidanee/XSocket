#include "Server.h"
#ifndef _WIN32
#include <signal.h>
#endif
int main()
{
#ifndef _WIN32
	sigset_t signal_mask;
	sigemptyset(&signal_mask);
	sigaddset(&signal_mask, SIGPIPE);
	if (pthread_sigmask(SIG_BLOCK, &signal_mask, NULL) == -1)
		perror("SIGPIPE");
#endif
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