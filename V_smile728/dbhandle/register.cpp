#include <sys/socket.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
using namespace std;

int registration(char name[16], char passwd[24])
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "xirujun", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	if(mysql_real_query(&mysql, "select username from users", (unsigned long)strlen("select username from users")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return 0;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return 0;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname; 
		pname = strcmp(row[0], name);
		if(pname == 0)
		{
			cout << "this username exist!" << endl;
			return 2;
		}
		//break;
	}

	string spwd, sname;
	spwd = passwd; 
	sname = name;
	string comd = "insert into users (username,password) values('"+sname+"','"+spwd+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "register success!" << endl;
		return 1;
	}

	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int main(void)
{
	int IsRegsuccess;
	IsRegsuccess = registration("finder", "123");
	if(IsRegsuccess == 1)
		cout << "registration success!" << endl;
	else
		cout << "registration failure!" << endl;

	return 0;
}

