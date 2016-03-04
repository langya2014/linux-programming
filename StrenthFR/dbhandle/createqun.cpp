#include <sys/socket.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
using namespace std;

#define maxdscrb 128
int createqun(char qunname[32], char qunmaster[32], char qunportrt[2], char qundscrb[maxdscrb])
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

	if(mysql_real_query(&mysql, "select qunname from qun_tab", (unsigned long)strlen("select qunname from qun_tab")))
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
		pname = strcmp(row[0], qunname);
		if(pname == 0)
		{
			cout << "this qun name exist!" << endl;
			return 2;
		}
		//break;
	}

	string smaster, sname, sdscrb;
	smaster = qunmaster; 
	sname = qunname;
	sdscrb = qundscrb;
	string sportrt = qunportrt;
	string comd = "insert into qun_tab (qunname,qunmaster, qunportrait, qundescrible) values('"+sname+"','"+smaster+"','"+sportrt+"','"+sdscrb+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "create qun success!" << endl;
		return 1;
	}

	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int main()
{
	cout << "compilation success!" <<endl;
	return 0;
}
