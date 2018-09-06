#ifndef __XISERVEREVENT_H__
#define __XISERVEREVENT_H__

#include "XCommon.h"

class XIServerEvent
{
public:
	virtual void AddTask(std::function<void()> pTask) = 0;
};

#endif