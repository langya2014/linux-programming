#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <mysql.h>
#include <stdlib.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
using namespace std;

#define maxrecdlen 128

char *gettime(void)
{
	time_t now;
	//char *ctime();

	(void) time(&now);
	return ctime(&now);
}

int addchatrecord(char username[32], char qunname[32], char record[maxrecdlen])
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

	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
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
		//char qunID[4];
		char qunname[32];
		//char recordno[12];
		string recordno;  
		char *qunID;
		qunID = (char *)malloc(4*sizeof(char));
		pname = strcmp(row[1], qunname);
		if(pname == 0)
		{
			qunID = row[0];
			switch((int)(*qunID)){
				case '1':
					recordno = "qun1_record";
					break;
				case '2':
					recordno = "qun2_record";
					break;
				case '3':
					recordno = "qun3_record";
					break;
				case '4':
					recordno = "qun4_record";
					break;
				case '5':
					recordno = "qun5_record";
					break;
				case '6':
					recordno = "qun6_record";
					break;
				case '7':
					recordno = "qun7_record";
					break;
				case '8':
					recordno = "qun8_record";
					break;
				case '9':
					recordno = "qun9_record";
					break;
				/*case '10':
					recordno = "qun10_record";
					break;*/
				default:
					cout << "exceed the maximun numberof the qun, error!" << endl;
			}
			
			string squnname, suser, squnno, stime, srecd;
			squnname = qunname; 
			suser = username;
			squnno = recordno;
			stime = gettime();
			srecd = record;
			string comd = "insert into '"+squnno+"' (qunname, username, cur_time, arecord) values('"+squnname+"','"+suser+"', '"+stime+"','"+srecd+"')";
			cout << comd.c_str() << endl;
			if(mysql_query(&mysql, comd.c_str()))
			{
				cout << "mysql_query failure!" <<endl;
				return 0;
			}
			else
			{
				cout << "add chat record success!" << endl;
				return 1;
			}	
			
		}
		free(qunID);
		//break;
	}
	
		
	mysql_free_result(res);
	mysql_close(&mysql);

}

int main()
{
	cout << "compilation success!" <<endl;
	return 0;
}
