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

my_ulonglong XMariaDBConnect::SearchStudent()
{
	const char *query = "select * from Students where School = 'Mide'";
	if (mysql_real_query(_pConnect, query, strlen(query)))
		show_error(_pConnect);

	MYSQL_RES *ret = mysql_store_result(_pConnect);
	if (ret == NULL)
		show_error(_pConnect);

	my_ulonglong num = mysql_num_rows(ret);

	mysql_free_result(ret);

	return num;
}

void XMariaDBConnect::show_error(MYSQL *mysql)
{
	printf("Error(%d) [%s] \"%s\"",
		mysql_errno(mysql),
		mysql_sqlstate(mysql),
		mysql_error(mysql));
	mysql_close(mysql);
	exit(-1);
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