#ifndef __XMARIADBCONNECT_H__
#define __XMARIADBCONNECT_H__

#include "XCommon.h"
#include <mysql.h>

class XMariaDBConnect
{
public:
	XMariaDBConnect();
	~XMariaDBConnect();

	my_ulonglong SearchStudent();

private:
	void show_error(MYSQL *mysql);
	void Init();
	void Done();

private:
	MYSQL* _pConnect;
};

#endif