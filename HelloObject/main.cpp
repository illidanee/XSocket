#include "XObjectManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <thread>

const int tCount = 4;
const int mCount = 100008;
const int nCount = mCount / tCount;

void OtherThread()
{
	AAA* pData[nCount];
	for (int i = 0; i < nCount; ++i)
	{
		pData[i] = AAA::CreateObject();
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
	for (int i = 0; i < nCount; ++i)
	{
		AAA::DeleteObject(pData[i]);
	}
}

int main()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> begin = std::chrono::high_resolution_clock::now();

	std::thread threads[tCount];
	for (int i = 0; i < tCount; ++i)
	{
		threads[i] = std::thread(OtherThread);

	}
	for (int i = 0; i < tCount; ++i)
	{
		threads[i].join();
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
	printf("time = %d ms \n", (int)time.count());


	return 0;
}