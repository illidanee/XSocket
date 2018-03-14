#include <stdlib.h>
#include <stdio.h>
#include <memory.h>


#include <chrono>
#include <thread>
#include <mutex>

#include <windows.h>

const int tCount = 4;
const int mCount = 100000;
const int nCount = mCount / tCount;

void OtherThread()
{
	char* pData[nCount];
	for (int i = 0; i < nCount; ++i)
	{
		pData[i] = new char[rand() % 128 + 1];
	}
	for (int i = 0; i < nCount; ++i)
	{
		delete[] pData[i];
	}
	std::this_thread::sleep_for(std::chrono::microseconds(1));
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

	system("pause");
	return 0;
}