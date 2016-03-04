#include <sys/socket.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
using namespace std;

int joinqun(char username[32], char qunname[32])
{
	MYSQL mysql;
	//MYSQL_RES *res, res2;
	//MYSQL_ROW row, row2;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "xirujun", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		return 0;
	}

	/*if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		return;
	}*/

	string squnname, suser;
	squnname = qunname; 
	suser = username;
	string comd = "insert into user_qun (username, qunname) values('"+suser+"','"+squnname+"')";
	cout << comd.c_str() << endl;
	if(mysql_query(&mysql, comd.c_str()))
	{
		cout << "mysql_query failure!" <<endl;
		return 0;
	}
	else
	{
		cout << "join in the qun success!" << endl;
		return 1;
	}
		
	//mysql_free_result(res);
	mysql_close(&mysql);

}

int main()
{
	cout << "compilation success!" <<endl;
	return 0;
}
