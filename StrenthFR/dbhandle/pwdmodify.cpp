#include <sys/socket.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
using namespace std;

int pwdmodify(char name[16], char newpwd[24])
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

	if(mysql_real_query(&mysql, "select * from users", (unsigned long)strlen("select * from users")))
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
		pname = strcmp(row[1], name);
		if(pname == 0)
		{
			string spwd, sname;
			spwd = newpwd; 
			sname = name;
			string comd = "update users set password='"+spwd+"' where username='"+sname+"'";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				
				cout << "mysql_query failure!" << endl;
				return 0;
			}
			else
			{
				cout << "passwd modify success!" << endl;
				return 1;
			}
			//row[9] = newpwd;
			//cout << "psd modify success!" << endl;
			//return 1;	
		}
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);

	//return 0;	
}

int main(void)
{
	int IsModsuccess;
	IsModsuccess = pwdmodify("name", "123456");
	if(IsModsuccess == 1)
		cout << "psd modify success!" << endl;
	else
		cout << "psd modify failure!" << endl;

	return 0;
}


