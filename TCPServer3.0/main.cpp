#include "MyServer.h"
#include "XSignal.h"

#ifndef _WIN32
#include <signal.h>
#endif

int main()
{
//ȥ���ж��źŹرշ�������
#ifndef _WIN32
	//sigset_t signal_mask;
	//sigemptyset(&signal_mask);
	//sigaddset(&signal_mask, SIGPIPE);
	//if (pthread_sigmask(SIG_BLOCK, &signal_mask, NULL) == -1)
	//	perror("SIGPIPE");
	signal(SIGPIPE, SIG_IGN);
#endif

	//����������
	//int nSize = sizeof(MyServer);
	MyServer* server = new MyServer;

	//����������
	server->Start();

	while (true)
	{
		char buffer[32] = {};
		scanf("%s", buffer);
		if (0 == strcmp(buffer, "exit"))
		{
			server->Stop();
			break;
		}
		else if (0 == strcmp(buffer, "Exit"))
		{
			server->Stop();
			break;
		}
	}

	return 0;
}