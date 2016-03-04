#include <sys/socket.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
using namespace std;

char* deletequn(char qunmaster[32])
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "xirujun", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return;
	}

	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname;
		char qunname[32]; 
		pname = strcmp(row[2], qunmaster);
		if(pname == 0)
		{
			qunname = row[1];
			string smaster;
			smaster = qunmaster;
			string comd = "delete from qun_tab where qunmaster='"+smaster+"'";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				
				cout << "delete qun failure!" << endl;
				return;
			}
			else
			{
				cout << "delete qun success!" << endl;
				//return qunname;
			}
		}
		//break;
	}

	if(mysql_real_query(&mysql, "select * from user_qun", (unsigned long)strlen("select * from user_qun")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return;
	}
	
	res2 = mysql_store_result(&mysql);
	if(res2 == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return;
	}

	while(row2 = mysql_fetch_row(res2))
	{
		//int pmaster;
		//char qunname[32]; 
		//pname = strcmp(row[2], qunmaster);
		if(strcmp(row[2], qunname) == 0)
		{
			string squnname;
			squnname = qunname;
			string comd = "delete from user_qun where qunname='"+squnname+"'";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				
				cout << "delete information from user_qun failure!" << endl;
				return;
			}
			else
			{
				cout << "delete information from user_qun success!" << endl;
			}
		}
		//break;
	}
		
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);

	return qunname;	
}
