#include "XMariaDBConnect.h"


XMariaDBConnect::XMariaDBConnect()
	:
	_pConnect(nullptr)
{
	Init();
}

XMariaDBConnect::~XMariaDBConnect()
{
	Done();
}

int XMariaDBConnect::SearchStudentBySchoolAndStudentID(const char* pSchool, const char* pStudentID)
{
	char query[256] = {};
	sprintf(query, "select * from Students where School = '%s' and StudentID = '%s'", pSchool, pStudentID);
	if (mysql_real_query(_pConnect, query, (unsigned long)strlen(query)))
	{
		show_error(_pConnect);
		return -1;
	}

	MYSQL_RES* ret = mysql_store_result(_pConnect);
	if (ret == NULL)
	{
		show_error(_pConnect);
		return -2;
	}

	my_ulonglong num = mysql_num_rows(ret);

	mysql_free_result(ret);

	return (int)num;
}

int XMariaDBConnect::UpdateStudentBySchoolAndStudentID(const char* pDevicel, const char* pSchool,
	const char* pSubject, const char* pMajor,
	const char* pStudentID, const char* pUserName,
	const char* pPhoneNumber, const char* pPassword,
	const char* pDeviceName, const char* pDeviceType)
{
	char query[256] = {};
	sprintf(query, "update Students \
					set Device = '%s', Subject = '%s', Major = '%s', UserName = '%s', PhoneNumber = '%s', Password = '%s', DeviceName = '%s', DeviceType = '%s' \
					where School = '%s' and StudentID = '%s'", 
					pDevicel, pSubject, pMajor, pUserName, pPhoneNumber, pPassword, pDeviceName, pDeviceType, pSchool, pStudentID);

	if (mysql_real_query(_pConnect, query, (unsigned long)strlen(query)))
	{
		show_error(_pConnect);
		return -1;
	}

	my_ulonglong num = mysql_affected_rows(_pConnect);
		
	return (int)num;
}

int XMariaDBConnect::SearchStudentByUserNameAndPassword(const char* pDevicel, const char* pUserName, const char* pPassword)
{
	char query[256] = {};
	sprintf(query, "select * from Students where UserName = '%s' and Password = '%s'", pUserName, pPassword);
	if (mysql_real_query(_pConnect, query, (unsigned long)strlen(query)))
	{
		show_error(_pConnect);
		return -1;
	}

	MYSQL_RES* ret = mysql_store_result(_pConnect);
	if (ret == NULL)
	{
		show_error(_pConnect);
		return -2;
	}

	my_ulonglong num = mysql_num_rows(ret);

	mysql_free_result(ret);

	return (int)num;
}

void XMariaDBConnect::show_error(MYSQL *mysql)
{
	XInfo("Error(%d) [%s] \"%s\"",
		mysql_errno(mysql),
		mysql_sqlstate(mysql),
		mysql_error(mysql));
	//mysql_close(mysql);
	//exit(-1);
}

void XMariaDBConnect::Init()
{
	_pConnect = mysql_init(nullptr);
	if (!mysql_real_connect(_pConnect, "192.168.0.90", "aoyi", "yang", "aoyi", 0, "/var/run/mysqld/mysqld.sock", 0))
		show_error(_pConnect);
}

void XMariaDBConnect::Done()
{
	mysql_close(_pConnect);
	_pConnect = nullptr;
}