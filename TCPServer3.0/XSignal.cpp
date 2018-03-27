#include "XSignal.h"

#include <chrono>
#include <thread>

void XSignal::Sleep()
{
	_Wait = true;
}

void XSignal::Wait()
{
	while (_Wait)
	{
		std::chrono::microseconds t(1000);
		std::this_thread::sleep_for(t);
	}
}

void XSignal::Wake()
{
	_Wait = false;
}