#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void show_error(MYSQL *mysql)
{
	printf("Error(%d) [%s] \"%s\"",
		mysql_errno(mysql),
		mysql_sqlstate(mysql),
		mysql_error(mysql));
	mysql_close(mysql);
	exit(-1);
}

int main(int argc, char *argv[])
{
	MYSQL* mysql = mysql_init(NULL);

	if (!mysql_real_connect(mysql, "192.168.0.90", "aoyi", "yang", "aoyi", 0, "/var/run/mysqld/mysqld.sock", 0))
		show_error(mysql);

	const char *query = "select * from Students where School = 'Mide'";
	if (mysql_real_query(mysql, query, strlen(query)))
		show_error(mysql);

	MYSQL_RES *ret = mysql_store_result(mysql);
	if (ret == NULL)
		show_error(mysql);

	//MYSQL_ROW row = mysql_fetch_row(ret);

	my_ulonglong num = mysql_num_rows(ret);
	printf("-------------- num = %d \n", num);
	//printf("-------------- num = %d | %s : %s : %s : %s : %s : %s |\n", num, row[0], row[1], row[2], row[3], row[4], row[5]);

	mysql_free_result(ret);

	mysql_close(mysql);

	return 0;
}
