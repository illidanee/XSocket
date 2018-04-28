#ifndef __XMARIADBCONNECT_H__
#define __XMARIADBCONNECT_H__

#include "XCommon.h"
#include <functional>
#include <mysql.h>

class XMariaDBConnect
{
public:
	XMariaDBConnect();
	~XMariaDBConnect();

	int SearchStudentBySchoolAndStudentID(const char* pSchool, const char* pStudentID);
	int UpdateStudentBySchoolAndStudentID(const char* pDevicel, const char* pSchool,
		const char* pSubject, const char* pMajor,
		const char* pStudentID, const char* pUserName,
		const char* pPhoneNumber, const char* pPassword,
		const char* pDeviceName, const char* pDeviceType);
	int SearchStudentByUserNameAndPassword(const char* pDevicel, const char* pUserName, const char* pPassword);

private:
	void show_error(MYSQL *mysql);
	void Init();
	void Done();

private:
	MYSQL* _pConnect;
};

#endif