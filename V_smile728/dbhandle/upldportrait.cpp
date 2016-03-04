#include <sys/socket.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

int upldptr(char name[16], char picname[24])
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
			FILE *fp;
			if((fp=fopen("testpic.png","rb"))==NULL)
			{
				cout << "cannot open picture!" << endl;
				return 0;
			}
			
			char picbuf[1024]={0};
			while(1)
			{
				int nRead = fread(picbuf, 1024, 1, fp);
				if(nRead<1 && ferror(fp)!=0)
				{
					cout << "failed to read picture!" << endl;
					return 0;
				}
				
				//upload portrait
				string spicbuf, sname;
				//spwd = newpwd; 
				sname = name;
				spicbuf = picbuf;
				//string spicname = picname;
				string comd = "update users set portrait='"+spicbuf+"' where username='"+sname+"'";
				//string comd = "update users set portrait='"+spicname+"' where username='"+sname+"'";
				cout << comd.c_str() << endl;
				if(mysql_query(&mysql, comd.c_str()))
				{
				
					cout << "mysql_query failure!" << endl;
					return 0;
				}
				else
				{
					cout << "upload picture success!" << endl;
					return 1;
				}

				if(feof(fp))
				{
					cout << "successfully upload the picture to table users!" << endl;
					break;
				}

			}
			
			/*string comd = "update users set password='"+spwd+"' where username='"+sname+"'";
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
			}*/
			//row[9] = newpwd;
			//cout << "psd modify success!" << endl;
			//return 1;

			fclose(fp);	
		}
	}
	
	
	mysql_free_result(res);
	mysql_close(&mysql);


	//return 0;	
}

int main(void)
{
	int IsUploadsuccess;
	IsUploadsuccess = upldptr("name", "testpic.png");
	if(IsUploadsuccess == 1)
		cout << "upload portrait success!" << endl;
	else
		cout << "upload portrait failure!" << endl;

	return 0;
}

