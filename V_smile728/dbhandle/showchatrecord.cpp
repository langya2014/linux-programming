#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <mysql.h>
#include <stdlib.h>
#pragma comment(lib, "libmysql.lib")

#include <iostream>
#include <string.h>
using namespace std;

#define maxmsglen 128

struct RECORD{
	string username;
	string cur_time;
	string message;
};

struct RECORD record[20];
//struct RECORD *p_record;
//record = (struct RECORD *)malloc(20*sizeof(struct RECORD)); 	//remember free the memory;
//p_record = record;

struct RECORD *showchatrcd(string username, string qunname)
{
	MYSQL mysql;
	MYSQL_RES *res, *res2;
	MYSQL_ROW row, row2;

	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "127.0.0.1", "root", "vsmile", "VSimle_db", 3306, 0, 0))
	{
		cout << "mysql_real_connect failure!" << endl;
		//return;
	}

	if(mysql_real_query(&mysql, "select * from qun_tab", (unsigned long)strlen("select * from qun_tab")))
	{
		cout << "mysql_real_query failure!" <<endl;
		//return;
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		cout << "mysql_store_result failure!" << endl;
		//return;
	}

	while(row = mysql_fetch_row(res))
	{
		int pname;
		char qunname[32];
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
			
			string squnno;
			squnno = recordno;
			//string comd = "insert into '"+squnno+"' (qunname, username, cur_time, arecord) values('"+squnname+"','"+suser+"', '"+stime+"','"+srecd+"')";
			string comd = "select * from '"+squnno+"'";
			cout << comd.c_str() << endl;
			if(mysql_real_query(&mysql, comd.c_str(), (unsigned long)strlen(comd.c_str())))
			{
				cout << "mysql_real_query failure!" <<endl;
				return 0;
			}
			else
			{
				cout << "query chat record success!" << endl;
			}	

			res2 = mysql_store_result(&mysql);
			if(res2 == NULL)
			{
				cout << "mysql_store_result failure!" << endl;
				return 0;
			}
			
			int i = 0;
			while(row = mysql_fetch_row(res))
			{
				/*(record[i]).username = row[1];
				(record[i]).cur_time = row[2];
				(record[i]).message = row[3];*/

				/*char *usrname, *curtime, *msg;
				usrname = (char *)malloc(32*sizeof(char));
				curtime = (char *)malloc(32*sizeof(char));
				msg = (char *)malloc(maxmsglen*sizeof(char));
				usrname = row[1];
				curtime = row[2];
				msg = row[3];
				(record[i]).username = usrname;
				(record[i]).cur_time = curtime;
				(record[i]).message = msg;*/
				
				struct RECORD *p_rcd;
				p_rcd = &(record[i]);
				//string row1 = row[1];

				p_rcd->username = row[1];
				p_rcd->cur_time = row[2];
				p_rcd->message = row[3];

				++i;
				if(i == 20)
				{
					/*free(usrname);
					free(curtime);
					free(msg);*/
					break;
				}
			}
				
		}
		
	}
	
	//free(precord);	
	mysql_free_result(res);
	mysql_free_result(res2);
	mysql_close(&mysql);

	return record;
}

int main()
{
	cout << "compilation success!" <<endl;
	//free(precord);
	return 0;
}
